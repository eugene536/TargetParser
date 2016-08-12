#include <Grammar.h>

using namespace gsx;

namespace target_parser {

void DeleteQuotes(char const * first, char const * last, std::string & value) {
    if ( first != last ) {
        if ( *first == '"' ) {
            ++first;
            --last;
        }

        value = std::string(first, last);
    }
}

struct my_chset : chset {
    my_chset (const char* sz)
        : chset(sz)
    {}

    ~my_chset() {
        free(_sz);
    }
};

rule ws;
rule pattern_name;
rule quoted_pattern;

void Terminal::init() {
    __rule = make_bind(&Terminal::_pattern_name, pattern_name) >> ws >> chlit('=') >> ws >> make_bind(&Terminal::_exp, quoted_pattern, DeleteQuotes) >> ws
    ;
}

void Terminal::Calculate(int pos) {
    _vertex_id = pos;
}

bool Terminal::SetValue(int, bool value) {
    return _matched = value;
}

void Terminal::ForEachVertex(const Func& func) const {
    func({_vertex_id, _pattern_name, _exp});
}

std::ostream& operator<<(std::ostream& out, const Terminal& terminal) {
    return out << "\"" << terminal._pattern_name << " " << terminal._exp << "\"";
}

void Symbols::init () {
    __rule = (chlit('(') >> ws >> ie(&Symbols::_exp) >> ws >> chlit(')')
             | ie(&Symbols::_term)) >> ws ;
}

Symbols::Symbols(const Symbols &oth) {
    *this = oth;
}

void Symbols::Calculate(int pos) {
    if (!_exp.empty()) {
        _exp->Calculate(pos);
        _size = _exp->_size;
        _matched = _exp->_matched;
    } else if (!_term.empty()) {
        _term->Calculate(pos);
        _size = 1;
        _matched = _term->_matched;
    } else
        assert(("unreachable state", false));
}

bool Symbols::SetValue(int pos, bool value) {
    assert(_exp.empty() || _term.empty());
    if (!_exp.empty())
        return _matched = _exp->SetValue(pos, value);
    else if (!_term.empty())
        return _matched = _term->SetValue(pos, value);

    assert(("unreachable state", false));
}

void Symbols::ForEachVertex(const Func &func) const {
    if (!_exp.empty())
        _exp->ForEachVertex(func);
    else if (!_term.empty())
        _term->ForEachVertex(func);
    else
        assert(("unreachable state", false));
}

Symbols& Symbols::operator=(const Symbols &rhs) {
    _matched = rhs._matched;
    _size = rhs._size;

    if (!rhs._exp.empty())
        _exp = new Expression(*rhs._exp.ptr());
    else if (!rhs._term.empty())
        _term = new Terminal(*rhs._term.ptr());

    return *this;
}

std::ostream& operator<<(std::ostream& out, const Symbols& symbols) {
    if (!symbols._exp.empty())
        return out << *(symbols._exp);
    else if (!symbols._term.empty())
        return out << *(symbols._term);

    assert(("unreachable state", false));
}

void AndPart::init() {
    __rule =
        ((chlit('!') >> ie(&AndPart::_not_sym) ) | ie(&AndPart::_sym)) >> ws
    ;
}

void AndPart::Calculate(int pos) {
    if (!_sym.empty()) {
        _sym->Calculate(pos);
        _size = _sym->_size;
        _matched = _sym->_matched;
    } else if (!_not_sym.empty()) {
        _not_sym->Calculate(pos);
        _size = _not_sym->_size;
        _matched = !_not_sym->_matched;
    } else
        assert(("unreachable state", false));
}

bool AndPart::SetValue(int pos, bool value) {
    assert(_sym.empty() || _not_sym.empty());

    if (!_sym.empty())
        return _matched = _sym->SetValue(pos, value);
    else if (!_not_sym.empty())
        return _matched = !_not_sym->SetValue(pos, value);

    assert(("unreachable state", false));
}

void AndPart::ForEachVertex(const Func &func) const {
    if (!_sym.empty())
        _sym->ForEachVertex(func);
    else if (!_not_sym.empty())
        _not_sym->ForEachVertex(func);
    else
        assert(("unreachable state", false));
}

AndPart::AndPart(const AndPart &oth) {
    *this = oth;
}

AndPart& AndPart::operator=(const AndPart &rhs) {
    assert(_sym.empty() || _not_sym.empty());

    _size = rhs._size;
    _matched = rhs._matched;

    if (!rhs._sym.empty())
        _sym = new Symbols(*rhs._sym.ptr());
    else if (!rhs._not_sym.empty())
        _not_sym = new Symbols(*rhs._not_sym.ptr());
    return *this;
}

std::ostream& operator<<(std::ostream& out, const AndPart& andPart) {
    if (!andPart._not_sym.empty()) out << "!";

    return
        out << "("
            <<  (andPart._sym.empty() ? *andPart._not_sym : *andPart._sym )
            << ")";
}

void OrPart::init() {
    __rule =
        make_bind_ie(&OrPart::_ands, AndPart::__rule) >> *(ws >> chlit('&') >> ws >> make_bind_ie(&OrPart::_ands, AndPart::__rule)) >> ws;
    ;
}

void OrPart::Calculate(int pos) {
    _matched = true;
    _size = 0;

    for (AndPart& a: _ands) {
        a.Calculate(pos + _size);
        _size += a._size;
        _matched &= a._matched;
    }
}

bool OrPart::SetValue(int pos, bool value) {
    assert(pos < _size && _ands.size());
    int l = 0;
    int r = 0;
    _matched = true;
    for (AndPart& a: _ands) {
        l = r;
        r += a._size;

        if (l <= pos && pos < r) {
            a.SetValue(pos - l, value);
        }

        _matched &= a._matched;
    }

    return _matched;
}

void OrPart::ForEachVertex(const Func &func) const {
    for (const AndPart& a: _ands)
        a.ForEachVertex(func);
}

std::ostream& operator<<(std::ostream& out, const OrPart& orPart) {
    for (size_t i = 0; i < orPart._ands.size(); ++i) out << "(";

    for (size_t i = 0; i < orPart._ands.size() - 1; ++i) {
        out << orPart._ands[i] << ") & ";
    }
    return  out << orPart._ands.back() << ")";
}

void Expression::init() {
    __rule =
        ws >> make_bind_ie(&Expression::_ors, OrPart::__rule) >> *(ws >> chlit('|') >> ws >> make_bind_ie(&Expression::_ors, OrPart::__rule)) >> ws
    ;
}

void Expression::Calculate(int pos) {
    _matched = false;
    _size = 0;

    for (OrPart& o: _ors) {
        o.Calculate(pos + _size);
        _size += o._size;
        _matched |= o._matched;
    }
}

bool Expression::SetValue(int pos, bool value) {
    assert(pos < _size && _ors.size());
    int l = 0;
    int r = 0;
    _matched = false;
    for (OrPart& o: _ors) {
        l = r;
        r += o._size;

        if (l <= pos && pos < r) {
            o.SetValue(pos - l, value);
        }

        _matched |= o._matched;
    }

    return _matched;
}

void Expression::ForEachVertex(const Func &func) const {
    for (const OrPart& o: _ors)
        o.ForEachVertex(func);
}

std::ostream& operator<<(std::ostream& out, const Expression& expression) {
    for (size_t i = 0; i < expression._ors.size(); ++i) out << "(";
    for (size_t i = 0; i < expression._ors.size() - 1; ++i) {
        out << expression._ors[i] << ") | ";
    }
    return out << expression._ors.back() << ")";
}

void Init() {
    quoted_pattern = chlit('"') >> *( (chlit('\\') >> anychar) | (anychar - chlit('"')) ) >> chlit('"');
    ws = *(chlit(' '));
    pattern_name = *(my_chset("a-z_"));

    Terminal::init();
    Expression::init();
    AndPart::init();
    OrPart::init();
    Symbols::init();
}

} // namespace target_parser

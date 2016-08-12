#include <PatternTree.h>
#include <Grammar.h>
#include <ostream>
#include <sstream>

namespace target_parser {

PatternTree::PatternTree()
    : _exp(new Expression)
{}

PatternTree::PatternTree(const PatternTree& rhs) {
    assert(_valid);

    *this = rhs;
}

PatternTree::~PatternTree() {
    delete _exp;
}

PatternTree& PatternTree::operator=(const PatternTree& rhs) {
    assert(_valid);

    _exp = new Expression(*rhs._exp);

    return *this;
}

void PatternTree::SetValue(int pos, bool value) {
    assert(_valid);

    _exp->SetValue(pos, value);
}

bool PatternTree::Calculate() const {
    assert(_valid);

//    _pb->Calculate();
    return _exp->_matched;
}

std::string PatternTree::ToString() const {
    assert(_valid);

    std::stringstream ss;
    ss << *this;
    return ss.str();
}

bool PatternTree::IsValid() const {
    return _valid;
}

void PatternTree::SetValid(bool value) {
    _valid = value;
}

std::ostream& operator<<(std::ostream& out, const PatternTree& pt) {
    return out << *(pt._exp);
}

void PatternTree::ForEachVertex(const Func& func) const {
    _exp->ForEachVertex(func);
}

} // namespace target_parser

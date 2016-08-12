#include <Parser.h>
#include <PatternTree.h>
#include <Grammar.h>

namespace target_parser {

PatternTree Parse(const std::string & text, int * pos_unparsed /* = nullptr */) {
    return Parse(text.c_str(), text.size(), pos_unparsed);
}

PatternTree Parse(char const * first, size_t len, int * pos_unparsed /* = nullptr */) {
    PatternTree pt;
    Expression& exp = *pt._exp;

    gsx::position f = first;
    gsx::position l = first + len;

    gsx::match hits = exp.parse(f, l);

    if (hits && (f == l)) {
        exp.Calculate();
    } else {
        if (pos_unparsed) {
            *pos_unparsed = f._max_first - first;
        }

        pt.SetValid(false);
    }

//    std::cerr << bool(hits) << std::endl;
//    std::cerr << "uparse:" << f._max_first << std::endl;
//    assert(false);

    return  pt;
}

} // namespace target_parser

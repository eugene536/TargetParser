#ifndef GRAMMAR_H
#define GRAMMAR_H
/*
Grammar: (Boost spirit syntax)
Expression    = OrPart >> *('|' >> OrPart)
OrPart        = AndPart >> *('&' >> AndPart)
AndPart       =  '!' >> Symbols | Symbols
Symbols       = '('  >> Expression  >> ')' | Terminal
Terminal      = pattern_name >> '=' >> quoted_string
quoted_string = chlit('"') >> *( (chlit('\\') >> anychar) | (anychar - chlit('"')) ) >> chlit('"');
pattern_name  = [a-z_]+
ws            = *(" ");
*/

#include <GSX/GSX.h>
#include <ostream>
#include <functional>
#include <InitTargetParser.h>

namespace target_parser {

struct Expression;

struct Terminal
    : gsx::ie_base<Terminal>
{
    static void init();

    void Calculate(int pos);
    bool SetValue(int, bool value);
    void ForEachVertex(const Func& func) const;

    friend std::ostream& operator<<(std::ostream& out, const Terminal& terminal);

    int _vertex_id;
    std::string _pattern_name;
    std::string _exp;

    bool _matched = false;
};

struct Symbols
    : gsx::ie_base<Symbols>
{
    static void init ();

    Symbols() = default;
    Symbols(const Symbols& oth);
    Symbols& operator=(const Symbols& rhs);

    void Calculate(int pos);
    bool SetValue(int pos, bool value);
    void ForEachVertex(const Func& func) const;

    friend std::ostream& operator<<(std::ostream& out, const Symbols& symbols);

    psc::smart_ptr<Expression> _exp;
    psc::smart_ptr<Terminal> _term;

    int _size = 0;
    bool _matched = false;
};

struct AndPart
    : gsx::ie_base<AndPart>
{
    static void init();

    AndPart() = default;
    AndPart(const AndPart& oth);
    AndPart& operator=(const AndPart& rhs);

    void Calculate(int pos);
    bool SetValue(int pos, bool value);
    void ForEachVertex(const Func& func) const;

    friend std::ostream& operator<<(std::ostream& out, const AndPart& andPart);


    psc::smart_ptr<Symbols> _sym;
    psc::smart_ptr<Symbols> _not_sym;

    int _size = 0;
    bool _matched = false;
};

struct OrPart
    : gsx::ie_base<OrPart>
{
    static void init();

    void Calculate(int pos);
    bool SetValue(int pos, bool value);
    void ForEachVertex(const Func& func) const;

    friend std::ostream& operator<<(std::ostream& out, const OrPart& orPart);

    std::vector<AndPart> _ands;

    int _size = 0;
    bool _matched = false;
};

struct Expression
    : gsx::ie_base<Expression>
{
    static void init();

    void Calculate(int pos = 0);
    bool SetValue(int pos, bool value);
    void ForEachVertex(const Func& func) const;

    friend std::ostream& operator<<(std::ostream& out, const Expression& expression);

    std::vector<OrPart> _ors;

    int _size = 0;
    bool _matched = false;
};

} // namespace target_parser

#endif

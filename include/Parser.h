#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <PatternTree.h>

/**
 * @defgroup TargetParser
 * @brief содержит функции для парсинга и структуры для работы с разобранным деревом
 *
 * @verbatim
   Пример валидной строки для разбора:
     ip = "123*" | host = "?google.\"ru" & !(imsdn = "10" & email = "*gmail" | phone = "*952*")

   Грамматика парсера:
     Expression     = OrPart ('|' OrPart)*
     OrPart         = AndPart ('&' AndPart)*
     AndPart        =  '!' Symbols | Symbols
     Symbols        = '('  Expression  ')' | Terminal
     Terminal       = pattern_name '=' quoted_pattern
     quoted_pattern = '"' ( ('\' anychar) | (anychar - '"') )* '"'
     pattern_name   = [a-z_]+
     ws             = (' ')*
   @endverbatim
 *
 * в main нужно подключить файл InitTargetParser.h и вызвать функцию target_parser::Init() <br>
 * в месте, где нужно распарсить, подключать файл Parser.h и вызывать функцию и target_parser::Parse(char const *, size_t, int *)
 *
 * @author Nemchenko Eugene
 * @date 12/08/2016
 */

/**
 * @ingroup TargetParser
 * @brief содержит декларации функций для парсинга target_parser::Parse(char const *, size_t, int *)
 *
 * @file Parser.h
 * @author Nemchenko Eugene
 * @date 12/08/2016
 */

/**
 *  @addtogroup TargetParser
 *  @{
 */

//! Содержит функции и структуры для работы с парсером
namespace target_parser {

/**
 * @see Parse(char const *, size_t, int *)
 */
PatternTree Parse(const std::string & text, int * pos_unparsed = nullptr);

/**
 * @brief разбирает строку по соответствующей грамматике и строит для нее дерево разбора
 * @param first указатель на начало строки
 * @param len длина строки
 * @param pos_unparsed если не равен nullptr, в случае ошибки (@link PatternTree::IsValid() IsValid @endlink == false), указывает на первый неразобранный символ
 * @return разобранное дерево, нужно проверить его, что он в валидном состоянии(строка распарсилась)
 */
PatternTree Parse(char const * first, size_t len, int * pos_unparsed = nullptr);

} // namespace target_parser

/*! @} End of Doxygen Groups*/

#endif // PARSER_H

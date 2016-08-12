#ifndef INITTARGETPARSER_H
#define INITTARGETPARSER_H

#include <string>
#include <ostream>
#include <functional>

/**
 * @ingroup TargetParser
 * @brief содержит структуру Vertex и декларацию функции target_parser::Init
 *
 * @file InitTargetParser.h
 * @author Nemchenko Eugene
 * @date 12/08/2016
 */

namespace target_parser {

/**
 * @brief класс представляющий терминальное состояние вершины в дереве разбора PatternTree
 */
struct Vertex {
    /**
     * @brief уникальный идентификатор вершины
     */
    int vertexId;

    /**
     * @brief имя паттерна ip, email ...
     */
    const std::string& pattern_name;

    /**
     * @brief сам паттерн
     */
    const std::string& pattern;

    /**
     * @brief выводит вершину, для дебага
     */
    friend std::ostream& operator<<(std::ostream& out, const Vertex& v) {
        return out << "id: " << v.vertexId << "; p_name: " << v.pattern_name << "; pattern: " << v.pattern;
    }
};

/**
 * @brief тип функтора который передается в метод PatternTree::ForEachVertex
 */
typedef std::function<void(const Vertex&)> Func;


/**
 * @brief инициализирует грамматику и парсер, нужно вызывать в main до использования функции @link target_parser::Parse(char const *, size_t, int *) Parse @endlink
 *
 */
void Init();

} // namespace target_parser

#endif // INITTARGETPARSER_H

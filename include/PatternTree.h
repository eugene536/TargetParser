#ifndef PATTERNTREE_H
#define PATTERNTREE_H

#include <string>
#include <ostream>
#include <functional>
#include <InitTargetParser.h>

/**
 * @ingroup TargetParser
 * @brief содержит класс представляющий дерево разбора target_parser::PatternTree
 *
 * @file PatternTree.h
 * @author Nemchenko Eugene
 * @date 12/08/2016
 */

namespace target_parser {
/**
 * @brief Класс представляющий разобранное выражение в виде дерева, возвращается из метода @link target_parser::Parse(char const *, size_t, int *) Parse @endlink
 */
class PatternTree
{
public:
    PatternTree();
    PatternTree(const PatternTree& rhs);
    ~PatternTree();

    /**
     * @brief полностью копирует дерево
     */
    PatternTree& operator=(const PatternTree& rhs);

    /**
     * @brief вызывает функцию func на каждой из терминальных вершин дерева, передавая ей состояние вершины
     * @param func функтор который будет вызываться
     */
    void ForEachVertex(const Func& func) const;

    /**
     * @brief обновляет значение в вершине с номером pos
     * @param pos номер вершины, который можно получить при вызове функции ForEachVertex(const Func& func) const
     * @param value новое значение для вершины
     */
    void SetValue(int pos, bool value);

    /**
     * @brief считает значение для всего логического выражения, представленного в виде дерева
     * @return значение логического выражения
     */
    bool Calculate() const;

    /**
     * @brief конвертирует дерево в строковое представление, расставляя скобки около всех операций
     * @return строковое представление дерева
     */
    std::string ToString() const;

    /**
     * @brief возвращает true, в случае если дерево валидно распарсилось и готово к использованию
     * @return готовность дерева к использованию
     */
    bool IsValid() const;

private:
    void SetValid(bool value);

private:
    struct Expression * _exp;
    bool _valid = true;

private:
    friend std::ostream& operator<<(std::ostream& out, const PatternTree& pt);

    friend PatternTree Parse(char const * first, size_t len, int * pos_unparsed);
};

} // namespace target_parser

#endif // PATTERNTREE_H

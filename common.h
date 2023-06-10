#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

#ifndef DPLL_COMMON_H
#define DPLL_COMMON_H

// A literal is a atomic formula (that contains a variable). Like in dimacs,
// + positive numbers denote the corresponding variables;
// - negative numbers denote the negations of the corresponding variables.
// Variables are numbered from 1.
typedef int literal; // 文字
#define POSITIVE(x) ((x) > 0)
#define NEGATIVE(x) ((x) < 0)
#define VAR(x) (((x) > 0) ? (x) : (-(x)))

// A clause is a list of literals (meaning their disjunction).子句为文字的析取
typedef std::vector<literal> clause;

// A formula is a list of clauses (meaning their conjunction).  cnf为子句的合取范式
// We also specify the total number of variables, as some of them may not occur in any clause!
struct formula {
    int num_variable;
    std::vector<clause> clauses;
    formula(int n, const std::vector<clause> &clauses) :
        num_variable(n), clauses(clauses) {
    }
};

// A satisfying model (interpretation).
// e.g. `model[i] = false` means variable `i` is assigned to false. 每个文字（布尔变量）取值的映射。
typedef std::unordered_map<int, bool> model;

#endif // DPLL_COMMON_H
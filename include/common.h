#include <vector>
#include <set>
#include <unordered_map>
#include <string>
#include <sstream>

#ifndef DPLL_COMMON_H
#define DPLL_COMMON_H

// A literal is a atomic formula (that contains a variable). Like in dimacs,
// + positive numbers denote the corresponding variables;
// - negative numbers denote the negations of the corresponding variables.
// Variables are numbered from 1.
typedef int literal; // 文字类型
#define POSITIVE(x) ((x) > 0)
#define NEGATIVE(x) ((x) < 0)
#define VAR(x) (((x) > 0) ? (x) : (-(x)))

// A clause is a list of literals (meaning their disjunction).
typedef std::set<literal> clause_t;
using clauses_t = std::vector<clause_t>;

// A formula is a list of clauses (meaning their conjunction).
// We also specify the total number of variables, as some of them may not occur in any clause!
struct formula {
    int num_variables;
    int num_clauses;
    std::vector<std::pair<int, int>> freq;
    clauses_t clauses;
    formula(int n, int m, const clauses_t &clauses, const std::vector<std::pair<int, int>> &frequency) :
        num_variables(n), num_clauses(m), clauses(clauses), freq(frequency) {
    }
};

// A satisfying model (interpretation).
// e.g. `model[i] = false` means variable `i` is assigned to false.
// model 映射每个 literal（布尔变量）实际取值,key is literal & value is assignment
typedef std::unordered_map<int, bool> model;

#endif // DPLL_COMMON_H
#ifndef DPLL_SOLVER_H
#define DPLL_SOLVER_H

#include "common.h"
#include <vector>
#include <set>

class DPLLSolver {
private:
    formula cnf_formula;       // the cnf_formula which parsed from xx.cnf file
    model result;              // result store every bool variable's value if CNF is satisfied
    std::set<int> assignments; // The assignments of variables.
public:
    DPLLSolver(const formula &cnf_formula) :
        cnf_formula(cnf_formula) {
    }
    bool solve(clauses_t clauses, std::set<int> assigns); // run DPLL algorithm to solve CNF formula
    model get_model();                                    // return result if CNF is satisfied

    /**
     * Test if the clauses are all empty, which means they are satisfiable. 如果剩余的clauses为空，则该CNF可满足.
     * @param clauses Current clause set.
     * @return True if empty.
     */
    bool isEmpty(clauses_t clauses);
    /**
     * Find the clause with only one literal. 找到单子句(只有一个文字的子句).
     * @param clauses Current clause set.
     * @return The alone literal, 0 if not exists.
     */
    int getUnitLiteral(clauses_t clauses);

    /**
     * Use the alone literal to simplify the clauses.
     * 利用单子句传播规则。简化CNF，从而进一步缩小搜索范围，加速求解过程.
     * @param clauses Current clause set.
     * @param literal The alone literal.
     * @return True if not contradicted.
     */
    bool unitPropagation(clauses_t &clauses, int unit_literal);
};
#endif // DPLL_DPLL_H
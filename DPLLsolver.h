#ifndef DPLL_SOLVER_H
#define DPLL_SOLVER_H

#include "common.h"
#include <vector>
#include <set>

/**
 * DPLL:
 *  member_data: formula cnf_formula; model result;
 *  merber_fun: bool solve(); model get_model();
 */
class DPLLSolver {
public:
    DPLLSolver(const formula &cnf_formula) :
        cnf_formula(cnf_formula) {
    }
    bool solve(clauses_t clauses, std::set<int> assigns); // run DPLL algorithm to solve CNF formula
    model get_model();                                    // return result if CNF is satisfied
private:
    /**
     * The assignments of variables.
     */
    std::set<int> assignments;
    /**
     * 如果剩余的clauses为空，则该CNF is satisfied
     * Test if the clauses are all empty, which means they are satisfiable.
     * @param clauses Current clause set.
     * @return True if empty.
     */
    bool isEmpty(clauses_t clauses);
    /**
     * 找到只有一个文字的clause
     * Find the clause with only one literal.
     * @param clauses Current clause set.
     * @return The alone literal, 0 if not exists.
     */
    int getUnitLiteral(clauses_t clauses);

    /**
     * 如果存在一个子句只有一个未赋值的文字，那么该文字必须为真，并更新剩余的子句
     * 根据给定的 literal 进行单子句传播。通过分离包含 literal 的子句和子句中的 -literal，函数可以减少子句集合中的冗余信息，从而缩小搜索空间
     * 利用单子句传播规则，可以迅速确定某些变量的取值，从而进一步缩小搜索范围，加速求解过程
     * Use the alone literal to simplify the clauses.
     * @param clauses Current clause set.
     * @param literal The alone literal.
     * @return True if not contradicted.
     */
    bool unitPropagation(clauses_t &clauses, int unit_literal);

private:
    formula cnf_formula; // the cnf_formula which parsed from xx.cnf file
    model result;        // result store every bool variable's value if CNF is satisfied
};
#endif                   // DPLL_DPLL_H
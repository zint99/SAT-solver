#ifndef CDCL_SOLVER_H
#define CDCL_SOLVER_H

#include "common.h"
#include <vector>
#include <set>
#include <stack>
#include <map>
#include <ostream>
#include <iterator>
#include <random>
#include <algorithm>

typedef std::set<int>::iterator iter_type;

/**
 * Literal assignment values.
 */
enum VALUE {
    POSITIVE,
    NEGATIVE,
    UNDEFINED
};

/**
 * UnitPropagate return state.
 * UPDATE - new unit found.
 * STABLE - new unit not found.
 * CONTRADICTED - contradiction found.
 */
enum STATE {
    UPDATED,
    STABLE,
    CONTRADICTED
};

/**
 * SAT solver based on CDCL.
 */
class CDCLSolver {
public:
    CDCLSolver(formula &f) :
        CNF_formula(f) {
    }

    /**
     * Main function to solve SAT problem by CDCL.
     * @return True if satisfiable.
     */
    bool solve();

    void printResult(std::ostream &output);

private:
    formula CNF_formula;

    /**
     * The assignment we has already set, include propagating and deciding.
     */
    std::vector<int> literalStack;

    /**
     * The assignment decisions we has already made.
     */
    std::stack<int> decisionStack;

    /**
     * Current variables assignments.
     */
    std::vector<VALUE> assignments;

    /**
     * The undefined variables.
     */
    clause_t undefinedSet;

    /**
     * The max times to restart.
     */
    int maxBackJumpingTimes = 10;

    /**
     * The times jumping back to an early decision.
     */
    int backJumpingTimes = 0;

    /**
     * 决策变量序随机选择：Select randomly in undefinedSet.
     * @param begin Start position.
     * @param end end position.
     * @return A iterator randomly distributed from begin to end.
     */
    iter_type randomlySelect(iter_type begin, iter_type end);

    /**
     * Set the variable value which makes the literal true.
     */
    void assign(int literal);

    /**
     * Unset the variable value on the top of literalStack.
     * @return The literal on the top of literalStack.
     */
    int unassign();

    /**
     * 子句学习：Learn new clause by contradiction.
     * @param unit The variable which will be assigned as opposite values in two different clauses.
     * @param i One clause.
     * @param j The other clause.
     * @return The new clause learned.
     */
    clause_t learn(int unit, int i, int j);

    /**
     * 布尔约束传播&冲突子句学习：Find the clauses with contradicted assignments expect one unknown literal.
     * Assign variables and learn new clause.
     * @return Propagation state and new clause learned.
     */
    STATE unitPropagate(std::set<int> &learnedClause);

    /**
     * 决策：Choose an undefined variable to assign.
     * @return False if undefinedSet is empty.
     */
    bool decide();

    /**
     * 重启：Clear literalStack if backJumpingTimes reaches maxBackJumpingTimes
     */
    void restart();

    /**
     * 非时序性回溯：Jump back to an early decision position according to the learned clauses.
     * @param learnedClause The new clause learned.
     * @return False if decisionStack is empty.
     */
    bool backJump(std::set<int> learnedClause);
};

#endif
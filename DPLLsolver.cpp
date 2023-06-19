#include "DPLLsolver.h"
#include <iostream>
#include <algorithm>
using namespace std;

bool DPLLSolver::isEmpty(clauses_t clauses) {
    for (auto clause : clauses) {
        if (!clause.empty()) {
            return false;
        }
    }
    return true;
}

int DPLLSolver::getUnitLiteral(clauses_t clauses) {
    for (auto clause : clauses) {
        if (clause.size() == 1) {
            return *clause.begin();
        }
    }
    return 0;
}

bool DPLLSolver::unitPropagation(clauses_t &clauses, int unit_literal) {
    for (auto it = clauses.begin(); it != clauses.end();) {
        if (it->count(unit_literal)) {         // clause中存在这个unit literal
            it = clauses.erase(it);            // 分离：消除包含unit literal的clause
        } else if (it->count(-unit_literal)) { // clause中存在-literal
            if (it->size() == 1) {
                return false;                  // 分离和单子句传播产生矛盾，说明前面的选择有问题，需要回溯选择另一个值重新搜索
            }
            it->erase(-unit_literal);          // 因为在单子句clause中literal已经取使其为true的值，所以-literal肯定为false，可以消除
            // mark:is there right?
            // if (it->empty()) {
            //     it = clauses.erase(it);
            // } else {
            //     it++;
            // }
            it++;
        } else {
            it++; // this clause doesnt have unit_literal,遍历下一个clause
        }
    }
    return true;
}

bool DPLLSolver::solve(clauses_t clauses, std::set<int> assigns) {
    while (1) {
        // find the unit_literal and do propagation
        int unitLiteral = getUnitLiteral(clauses);
        if (!unitLiteral) {
            break;
        }
        assigns.insert(unitLiteral);
        if (!unitPropagation(clauses, unitLiteral)) {
            return false; // 单元子句传播过程中发现矛盾，则返回 false，表示当前选择的赋值导致不可满足
        }
    }
    if (isEmpty(clauses)) {
        assignments = assigns;
        return true;
    }
    int p = *clauses.front().begin(); // literal
    clauses_t positive = cnf_formula.clauses;
    clauses_t negative = cnf_formula.clauses;
    positive.emplace_back(std::set<int>{p});
    negative.emplace_back(std::set<int>{-p});
    // 递归调用 solve(positive, assigns) 和 solve(negative, assigns)，分别尝试对 positive 和 negative 进行求解
    return solve(positive, assigns) || solve(negative, assigns);
}

model DPLLSolver::get_model() {
    for (int assignment : assignments) {
        int key = std::abs(assignment);
        bool value = (assignment > 0) ? true : false;
        this->result[key] = value;
    }
    return this->result;
}

#include "../include/DPLLsolver.h"
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
            return *clause.begin(); // return The alone literal
        }
    }
    return 0;
}

bool DPLLSolver::unitPropagation(clauses_t &clauses, int unit_literal) {
    for (auto it = clauses.begin(); it != clauses.end();) {
        if (it->count(unit_literal)) {         // this clause contains unit_literal,erase this clause from clauses then.
            it = clauses.erase(it);
        } else if (it->count(-unit_literal)) { // this clause contains -unit_literal
            if (it->size() == 1) {
                return false;                  // False if contradicted.
            }
            it->erase(-unit_literal);          // erase this literal from clause then.cuz it (i.e. -unit_literal) must be false.
            // unnecessary logic below
            // if (it->empty()) {
            //     it = clauses.erase(it);
            // } else {
            //     it++;
            // }
            it++;
        } else {
            it++; // this clause doesnt have unit_literal
        }
    }
    return true; // True if not contradicted.
}

bool DPLLSolver::solve(clauses_t clauses, std::set<int> assigns) {
    while (1) {
        // find the unit_literal and do propagation to reduce solution space
        int unitLiteral = getUnitLiteral(clauses);
        if (!unitLiteral) {
            break; // no unit_literal in CNF,run Variable decision
        }
        assigns.insert(unitLiteral);
        if (!unitPropagation(clauses, unitLiteral)) {
            return false; // contradicted occured,backtrack
        }
    }
    // CNF is satisfied if clauses is empty in the DPLL recursion search process
    if (isEmpty(clauses)) {
        assignments = assigns;
        return true;
    }
    int p = *clauses.front().begin(); // Variable decision simply selects the first variable of the first clause in order
    clauses_t positive = clauses;
    clauses_t negative = clauses;
    // Generate two branches of the recursion backtrace tree
    positive.emplace_back(std::set<int>{p});
    negative.emplace_back(std::set<int>{-p});

    // The disadvantage of the DPLL algorithm is that it only goes back one layer
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

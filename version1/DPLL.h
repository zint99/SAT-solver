#ifndef DPLL_DPLL_H
#define DPLL_DPLL_H

#include "common.h"

/**
 * DPLL:
 *  member_data: formula cnf_formula; model result;
 *  merber_fun: bool solve(); model get_model();
 */
class DPLL {
public:
    DPLL(const formula &cnf_formula) :
        cnf_formula(cnf_formula) {
    }
    bool solve();            // run DPLL algorithm to solve CNF formula
    model get_model() const; // return result if CNF is satisfied

private:
    formula cnf_formula; // the cnf_formula which parsed from xx.cnf file
    model result;        // result store every bool variable's value if CNF is satisfied
};
#endif                   // DPLL_DPLL_H
#ifndef DPLL_DPLL_H
#define DPLL_DPLL_H

#include "common.h"
// 判断 CNF 表达式的可满足性并获取模型
class DPLL {
public:
    DPLL(const formula &cnf_formula) :
        cnf_formula(cnf_formula) {
    }
    bool solve();      // 执行 DPLL 算法，判断 CNF 表达式的可满足性
    model get_model(); // 获取可满足性检查的结果，即找到的模型

private:
    formula cnf_formula; // CNF 表达式对象
    model result;        // 可满足性检查的模型结果（如果满足SAT的话则解析每个变量的取值得到模型）
};
#endif                   // DPLL_DPLL_H
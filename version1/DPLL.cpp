#include "DPLL.h"
#include <iostream>
using std::cout;
using std::endl;
/*
    DPLL::solve实现了一个基本的DPLL算法，其大致流程如下：
        1.初始化变量节点状态和赋值结果，将所有变量节点的状态初始化为未赋值`literalStatus[i] = 0;`，将所有变量的赋值初始化为真。`result.insert(std::make_pair(i, true));`
        2.从第一个变量开始，尝试依次将其取值赋为假、真，直到找到一个有效解或确定无解为止。
            - 深度优先搜索，先按照每个变量取值都为假一直搜，不行了再往真搜，当假和真的情况都搜索完且还不能满足cnf时，则回溯到之前还存在取值范围的变量继续DFS
            - 如果回溯到第一个变量，即root节点。还是不存在满足cnf的解集。则`return false`
        3.在每次更新变量取值后，检查当前的取值是否是解，即保证每个子句在当前文字的取值情况下都为真。
        4.如果当前取值不是解.
            - 若是由于当前存在值未确定的变量而不确定解，则移动指针（递归）到下一个变量赋值。
            - 若是由于当前变量取值不满足cnf，则继续换个方向搜或回溯到上一个未确定取值的变量，并将其状态重置为未赋值，再尝试下一个取值。
        5.如果当前取值是解，则返回true。
        6.如果当前取值不能确定，则递归到下一个变量取值。
*/
bool DPLL::solve() {
    /* initialization */
    int clauseNum = cnf_formula.clauses.size(); // clause数量
    int literalNum = cnf_formula.num_variable;  // literal数量
    for (int i = 1; i <= literalNum; i++)
        result.insert(std::make_pair(i, true)); // 如果CNF满足时 literala 取值，全部初始化为true | result[i] = true;
    int *literalStatus = new int[literalNum];   // 存储每个literal的赋值状态，0 表示未赋值，1 表示赋值为true，2 表示赋值为false。初始化为未赋值。
    for (int i = 0; i < literalNum; i++)
        literalStatus[i] = 0;
    int ptr2Literal = 0; // 指向当前执行赋值操作的 literal 指针
    /* ----- */

    // while loop 每一轮只处理一个变量的赋值，从 未赋值(x) -> true -> false -> break
    while (true) {
        // 根据 literal 状态做出相应更新
        if (literalStatus[ptr2Literal] == 2) // literal已经取过true和false两种赋值。只有回溯到根节点且仍然无法满足cnf时才会走这个分支
            break;
        else if (literalStatus[ptr2Literal] == 0) {
            literalStatus[ptr2Literal]++;   // literal assignment：未赋值 -> true
            result[ptr2Literal + 1] = true; // result中的key和ptr2Literal存在1的offset
        } else {
            literalStatus[ptr2Literal]++;   // literal assignment：true -> false
            result[ptr2Literal + 1] = false;
        }
        /*------------*/
        std::cout << "cur_literal is " << ptr2Literal + 1 << " val:" << result[ptr2Literal + 1] << std::endl;
        /* 检查当前的literal赋值组合是否使CNF satisfied，即保证每个clause在当前赋值下都为true */
        int solveStatus = 2;                         // solveStatus表示CNF status：0:unSat，1:sat，2:not Sure
        bool wholeCnfValue = true;                   // 整个cnf formula式子的取值

        for (int i = 0; i < clauseNum; i++) {        // 判断每个子句是否满足
            bool curClauseValue = false;             // 当前子句的状态，初始化为false
            bool any_notsure = false;                // 当前clause中是否存在还未赋值的literal
            int len = cnf_formula.clauses[i].size(); // clause length
            // 遍历子句所有文字，并判断if clause is satisfied
            for (int j = 0; j < len; j++) {
                int curClauseLiteral = cnf_formula.clauses[i][j]; // cnf中第i个子句的第j个文字
                if (VAR(curClauseLiteral) <= ptr2Literal + 1) {   // 判断当前literal是否已赋值，如果该literal已赋值则进入该分支
                    // 如果literal被赋值为true，且刚好在clause中也是true，则当前clause is satisfied
                    // 如果literal被赋值为false，且刚好在clause中也是false，则当前clause is satisfied
                    if ((POSITIVE(curClauseLiteral) && result[curClauseLiteral]) || (NEGATIVE(curClauseLiteral) && !result[VAR(curClauseLiteral)])) {
                        curClauseValue = true; // this clause is satisfied
                        break;                 // break and continue to next clause
                    }
                } else {
                    any_notsure = true;                      // this literal's value is not sure
                }
            }                                                // end of each clause literal

            wholeCnfValue = wholeCnfValue && curClauseValue; // CNF is satisfied when all clause is satisfied
            if (curClauseValue || any_notsure) {
                continue;                                    // (clause is satisfied || there is unsure literal)，则判断下个子句
            } else {
                solveStatus = 0;                             // 当前 literal 赋值组合下 CNF UNSAT，取消继续DFS开始回溯
                break;
            }
        } // end of each clause

        if (wholeCnfValue)
            solveStatus = 1; // CNF is satisfied
        /* 检查完毕 */

        if (solveStatus == 0) // CNF is not satisfied with 当前 literal 赋值组合
        {
            // 只有当前的文字走到2时才会回溯到之前的文字。否则将当前文字修改为2（true）
            // 当前文字已经取值过1和2，但仍然不是有效解，则回溯到上一个没有走到最后一步（取2）的文字，然后再将其取2尝试
            /*
                如果当前取值不是解（即solveStatus为0），则需要进行回溯操作。
                while循环语句会将ptr2Literal指针回溯到上一个状态为未赋值（即0）或者赋值为假（即1）的变量为止。
                这是因为，如果当前变量之后的所有变量都已经赋值为真，而仍然无法满足所有子句，那么回溯到这些变量也没有意义，因为它们已经被确定为真了，不再需要改变。
            */
            while (ptr2Literal > 0 && literalStatus[ptr2Literal] == 2)
                ptr2Literal--;
            // 回溯到最近的还未完全取完可能方案的文字后，其后的所有文字都要重置为未取值状态，以便重新取值生成新的解。因为在回溯后，之前尝试过的取值已经不再可行，需要重新尝试其他取值来寻找解
            for (int i = ptr2Literal + 1; i < literalNum; i++)
                literalStatus[i] = 0;
        } else if (solveStatus == 1)
            return true;   // 得到一组有效解，返回
        else
            ptr2Literal++; // 当前解为不能确定状态（因为any_notsure，可能还有变量没有取确定的值），dfs取下一个文字的值
    }                      // end of while
    return false;          // 取尽所有可能后都无法满足所有clause，只能return false
}

model DPLL::get_model() const {
    return this->result;
}

#include "DPLL.h"
/**
 *  dpll class
 *      -> （struct formula）cnf_formula;
 *          -> (int) num_variable：文字数量
 *          -> (std::vector<clause>) clauses：子句集合[[-1,2],[-2,3],[-1,-3]]
 *              -> clause：vector<literal> -> literal:int
 *      -> (std::unordered_map<int, bool> model) result
 */
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
    /* 初始化 */
    int clauseNum = cnf_formula.clauses.size(); // 子句数量
    int literalNum = cnf_formula.num_variable;  // 变量数量
    for (int i = 1; i <= literalNum; i++)
        result.insert(std::make_pair(i, true)); // 存储变量和赋值的映射关系，全部初始化为true  | result[i] = true;
    int *literalStatus = new int[literalNum];   // 存储每个变量的状态，初始化为未赋值。0 表示未赋值，1 表示赋值为真，2 表示赋值为假。
    for (int i = 0; i < literalNum; i++)
        literalStatus[i] = 0;
    int ptr2Literal = 0; // 指向当前处理的变量节点
    /* ----- */

    // while(true) 每一轮只处理一个变量的取值，从 未赋值 -> true -> false -> break
    while (true) {
        /* 更新变量取值 */
        if (literalStatus[ptr2Literal] == 2) // 变量已经到最后一种取值情况即无法继续更新赋值。只有回溯到根节点且取值无法满足cnf时才会走这个分支
            break;
        else if (literalStatus[ptr2Literal] == 0) {
            literalStatus[ptr2Literal]++;   // 节点状态：未赋值 -> 真
            result[ptr2Literal + 1] = true; // result中的key和ptr2Literal存在1的offset
        } else {
            literalStatus[ptr2Literal]++;   // 节点状态：真 -> 假
            result[ptr2Literal + 1] = false;
        }
        /*------------*/

        /* 检查当前的取值是否是解，若是解则保证每个clause在当前文字的取值情况下都为true */
        int solveStatus = 2;                         // solveStatus表示当前取值下解的状态：0 肯定不是解，1 肯定是解，2 不确定
        bool wholeCnfValue = true;                   // 整个cnf formula式子的取值

        for (int i = 0; i < clauseNum; i++)          // 遍历每个子句
        {
            bool curClauseValue = false;             // 当前子句的取值
            bool any_notsure = false;                // 是否有不确定取值的文字
            int len = cnf_formula.clauses[i].size(); // clause length
            // 遍历子句所有文字，并根据赋值结果判断子句的真值
            for (int j = 0; j < len; j++) {
                int curClauseLiteral = cnf_formula.clauses[i][j]; //  cnf中第i个子句的第j个文字取值
                if (VAR(curClauseLiteral) <= ptr2Literal + 1) {   // 判断当前文字是否为设置过取值的文字
                    if ((POSITIVE(curClauseLiteral) && result[curClauseLiteral]) || (NEGATIVE(curClauseLiteral) && !result[VAR(curClauseLiteral)])) {
                        // 如果这个文字在当前子句中取真POSITIVE(curClauseLiteral)，并且在结果result[curClauseLiteral]中也取真，则这个子句为真
                        // 如果这个文字在当前子句中取非NEGATIVE(curClauseLiteral)，并且在结果result[VAR(curClauseLiteral)]中取假，则这个子句为真
                        curClauseValue = true; // 当前子句取真
                        break;                 // 判断下一个子句
                    }
                } else {
                    any_notsure = true;                      // cnf中第i个子句的第j个文字是当前没有确定取值的文字
                }
            }                                                // end of each clause literal

            wholeCnfValue = wholeCnfValue && curClauseValue; // 更新整个cnf的取值，与每一个子句有关。
            if (curClauseValue || any_notsure) {
                continue;                                    // 当前子句为真 || 当前子句中存在未确定的文字；遍历下个子句
            } else {
                solveStatus = 0;                             // 此时文字取值组合肯定不是解
                break;
            }
        } // end of each clause

        if (wholeCnfValue)
            solveStatus = 1; // 如果所有子句都satisfied，则找到一组有效解
        /* 检查完毕 */

        if (solveStatus == 0) // 肯定不是解，回溯
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

model DPLL::get_model() {
    // TODO: your code here, or in the header file
    return this->result;
}

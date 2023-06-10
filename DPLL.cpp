// DPLL.cpp
#include "DPLL.h"

/**
 *  dpll class
 *      -> （struct formula）formula phi;
 *          -> (int) num_variable：文字数量
 *          -> (std::vector<clause>) clauses：子句集合[[-1,2],[-2,3],[-1,-3]]
 *              -> clause：vector<literal>
 *                  -> literal:int
 *      -> (std::unordered_map<int, bool>) model result
 */

bool DPLL::solve() {
    std::unordered_map<int, int> atomStatus; // 记录节点状态0，1，2
    /* 初始化 */
    int clause_num = phi.clauses.size();        // 子句数量
    int atomNum = phi.num_variable;             // 变量数量
    for (int i = 1; i <= atomNum; i++)
        result.insert(std::make_pair(i, true)); // 存储变量和其赋值的映射关系  | result[i] = true;
    int *nodeStatus = new int[atomNum];         // 存储每个变量节点的状态，初始化为未赋值。0 表示未赋值，1 表示赋值为假，2 表示赋值为真。
    for (int i = 0; i < atomNum; i++)
        nodeStatus[i] = 0;
    int ptr = 0; // 指向当前处理的变量节点
    // while(true) 这个循环只处理一个变量的取值，从 未赋值 -> false -> true -> break
    while (true) {
        // 根据节点状态的不同，更新相应的赋值结果
        if (nodeStatus[ptr] == 2) // 如果节点状态为真，则直接break
            break;
        else if (nodeStatus[ptr] == 0) {
            nodeStatus[ptr]++;       // 节点状态：未赋值 -> 假
            result[ptr + 1] = false; // result中的key和ptr存在1的offset
        } else {
            nodeStatus[ptr]++;       // 节点状态：假 -> 真
            result[ptr + 1] = true;
        }
        int solveStatus = 2; // 0 肯定不是解，1 肯定是解，2 不确定
        // 检查当前的取值是否是解，保证每个clause在当前文字的取值情况下都是true。
        bool wholeValue = true;              // 整个式子的真值

        for (int i = 0; i < clause_num; i++) // 遍历每个子句
        {
            bool currValue = false;          // 当前子句的取值
            bool any_notsure = false;        // 是否有不确定取值的文字
            int len = phi.clauses[i].size(); // clause length
            // 遍历子句所有文字，并根据赋值结果判断子句的真值
            for (int j = 0; j < len; j++) {
                int currvar = phi.clauses[i][j]; //  cnf中第i个子句的第j个文字取值
                if (VAR(currvar) <= ptr + 1) {   // 判断当前文字是否为设置过取值的文字
                    if ((POSITIVE(currvar) && result[currvar]) || (NEGATIVE(currvar) && !result[VAR(currvar)])) {
                        // 如果这个文字在当前子句中取真POSITIVE(currvar)，并且在结果result[currvar]中也取真，则这个子句为真
                        // 如果这个文字在当前子句中取非NEGATIVE(currvar)，并且在结果result[VAR(currvar)]中取假，则这个子句为真
                        currValue = true; // 当前子句取真
                        break;            // 判断下一个子句
                    }
                } else {
                    any_notsure = true;           // cnf中第i个子句的第j个文字是当前没有确定取值的文字
                }
            }                                     // end of each clause literal

            wholeValue = wholeValue && currValue; // 更新整个cnf的取值，与每一个子句有关。
            if (currValue || any_notsure) {
                continue;                         // 当前子句为真 || 当前子句中存在未确定的文字；遍历下个子句
            } else {
                solveStatus = 0;                  // 此时文字取值组合肯定不是解
                break;
            }
        } // end of each clause

        if (wholeValue)
            solveStatus = 1;
        // 检查完毕
        if (solveStatus == 0) // 肯定不是解，回溯
        {
            while (ptr > 0 && nodeStatus[ptr] == 2)
                ptr--;
            for (int i = ptr + 1; i < atomNum; i++)
                nodeStatus[i] = 0;
        } else if (solveStatus == 1) // 得到一组有效解
            return true;
        else
            ptr++; // 未知（因为可能还有变量没有取值），取下一个文字的值
    }              // end of while
    return false;
}

model DPLL::get_model() {
    // TODO: your code here, or in the header file
    return this->result;
}

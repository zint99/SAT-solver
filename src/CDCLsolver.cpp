#include "../include/CDCLsolver.h"
#include "../include/common.h"
#include <iostream>

iter_type CDCLSolver::randomlySelect(iter_type begin, iter_type end) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, std::distance(begin, end) - 1);
    std::advance(begin, dis(gen)); // ramdomly select,select the begin+dis(gen) item in the undefindSet
    return begin;
}

void CDCLSolver::assign(int literal) {
    // set the literal true,POSITIVE if literal > 0,NEGATIVE if literal < 0
    literalStack.push_back(literal);
    assignments[abs(literal)] = (literal > 0) ? POSITIVE : NEGATIVE;
    undefinedSet.erase(abs(literal));
}

int CDCLSolver::unassign() {
    int literal = literalStack.back();
    literalStack.pop_back();
    assignments[abs(literal)] = UNDEFINED;
    undefinedSet.insert(abs(literal));
    return literal;
}

std::set<int> CDCLSolver::learn(int unit, int i, int j) {
    // 此时unit在clauses[i]和clauses[j]中赋值发生冲突，需要从该冲突学习子句并回溯
    std::set<int> learnedClause;
    // 将两个发生冲突的子句拼接为一个新的clause，并在该拼接好的clause中分别删除 正负unit 得到学习到的新子句learnedClause
    std::set_union(CNF_formula.clauses[i].begin(),
                   CNF_formula.clauses[i].end(),
                   CNF_formula.clauses[j].begin(),
                   CNF_formula.clauses[j].end(),
                   std::inserter(learnedClause, learnedClause.begin()));
    learnedClause.erase(unit);
    learnedClause.erase(-unit);
    return learnedClause;
}

STATE CDCLSolver::unitPropagate(std::set<int> &learnedClause) {
    std::map<int, int> unitMap;
    int nbclauses = CNF_formula.clauses.size();
    for (int i = 0; i < nbclauses; i++) { // for every clause
        int unit = 0;                     // undefined literal in this clause
        std::set<int>::iterator it;
        for (it = CNF_formula.clauses[i].begin(); it != CNF_formula.clauses[i].end(); it++) {
            int literal = *it;

            VALUE value = assignments[abs(literal)];
            std::cout << "i: " << i << " literal: " << literal << " cur_value: " << value << std::endl;
            if (value == UNDEFINED && !unit) {
                unit = literal;
            } else if (!(value == POSITIVE && literal < 0 || value == NEGATIVE && literal > 0)) {
                /*
                    1.solve第一次执行，此时并未做出决策，literalStack中没有元素
                        1.若每个子句都非单子句，则直接break，导致unitMap.empty()，return STABLE.代表CNF中不存在单子句
                        2.若CNF中本来就存在单子句(即一个文字的clause)，则直接将整个CNF中寻找到的单子句中的文字都assign
                    2.solve中循环执行unitPropagate进行单子句传播。经过第一个步骤后，CNF中已经有assign的literal，在拥有这些literal的clause中进行单子句传播
                        1.若clause中存在assign的文字，并为true，则直接break表示此次传播跳过这个子句，因为拥有一个为true的literal就不用再考虑该clause
                        2.若clause中存在assign的文字，并为false，则不会break跳过，如果整个clause只有一个文字UNDEFINED且其他文字都false了，则赋值该literal(BCP的定义)
                        3.若clause中UNDIFINED的literal多于2个，则break跳过，传播不会影响该子句
                */
                break;
            }
        }
        if (it == CNF_formula.clauses[i].end()) {
            if (unit) {
                unitMap.insert({unit, i});
                std::cout << "unitMap.insert({unit, i}):  " << unit << " : " << i << std::endl;
            } else {
                return CONTRADICTED; // 所有变量的赋值导致某个clause为false，即当前赋值存在冲突，需要回溯
            }
        }
    }

    if (unitMap.empty()) {
        return STABLE; // 如果传播中得不到新的赋值了，则return STABLE，说明此时的CNF需要新一轮的decide
    } else {
        std::cout << "unitMap size:" << unitMap.size() << std::endl;
        for (auto tier : unitMap) {
            int unit, i;
            std::tie(unit, i) = tier;
            std::cout << "i: " << i << " unit: " << unit << std::endl;
            if (unitMap.find(-unit) != unitMap.end()) {
                int j = unitMap[-unit];
                learnedClause = learn(unit, i, j); // 说明两个变量在不同的clause中赋值相反，发生赋值冲突，需要从该冲突中学习字句并回溯
                CNF_formula.clauses.push_back(learnedClause);
                // TODO: Forget redundant CNF_formula.clauses
                return CONTRADICTED;
            } else {
                assign(unit);
            }
        }
        std::cout << "print unitMap end" << std::endl;
    }

    return UPDATED;
}

bool CDCLSolver::decide() {
    if (undefinedSet.empty()) {
        return false;
    }
    int literal = *randomlySelect(undefinedSet.begin(), undefinedSet.end());
    std::cout << "decision literal is: " << literal << std::endl;
    assign(literal);
    decisionStack.push(literal);
    return true;
}

void CDCLSolver::restart() {
    backJumpingTimes = 0;
    maxBackJumpingTimes++;
    while (!decisionStack.empty()) {
        decisionStack.pop();
    }
    while (!literalStack.empty()) {
        unassign();
    }
}

bool CDCLSolver::backJump(std::set<int> learnedClause) {
    std::cout << "backJump,the learnedClause.size() = " << learnedClause.size() << std::endl;
    std::cout << "the learnedClause is:" << std::endl;
    for (auto &it : learnedClause) {
        std::cout << it << " ";
    }

    if (decisionStack.empty()) {
        return false;
    }
    int decision = decisionStack.top();
    decisionStack.pop();
    int jumpingPoint = decision;
    for (int i = literalStack.size() - 1; i >= 0; i--) {
        if (learnedClause.count(-literalStack[i]) && literalStack[i] != decision
            || decisionStack.empty() || learnedClause.empty()) {
            break;
        }
        if (literalStack[i] == decisionStack.top()) {
            jumpingPoint = literalStack[i];
            decisionStack.pop();
        }
    }
    while (unassign() != jumpingPoint)
        ;
    assign(-decision);
    if (backJumpingTimes > maxBackJumpingTimes) {
        restart();
    } else {
        backJumpingTimes++;
    }
    return true;
}

bool CDCLSolver::solve() {
    // initialization of assignments & undefinedSet
    assignments.resize(CNF_formula.num_variables + 1, UNDEFINED);
    for (int i = 1; i <= CNF_formula.num_variables; i++) {
        undefinedSet.insert(i);
    }
    while (1) {
        std::set<int> learnedClause;
        STATE state = unitPropagate(learnedClause);              // CDCL重复进行BCP，直到没有新的单位子句，发生冲突时learnedClause保存冲突子句学习的结果
        if (state == CONTRADICTED && !backJump(learnedClause)) { // 如果BCP过程发生冲突，则利用学习到的子句进行非时序性回溯。如果回溯到顶层则说明该问题无解。
            return false;
        } else if (state == STABLE && !decide()) {               // 如果BCP无法找到可赋值变量，则进行变量决策。如果均被赋值，说明该问题有解。
            return true;
        }
    }
    return false;
}

void CDCLSolver::printResult(std::ostream &output) {
    for (int i = 1; i <= CNF_formula.num_variables; i++) {
        output << ((assignments[i] == POSITIVE) ? i : -i) << ' ';
    }
    output << std::endl;
}
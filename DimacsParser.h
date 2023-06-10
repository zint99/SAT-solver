/**
 *  Dimacs parser.  解析cnf合取范式
 */
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include "common.h"

#ifndef DPLL_DIMACSPARSER_H
#define DPLL_DIMACSPARSER_H

class DimacsParser {
public:
    /**
     * Parse a dimacs file.
     * @param file_name dimacs file name
     * @return a parsed formula (if succeeds)
     */
    static formula parse(const std::string &file_name) {
        // std::ifstream用于读取文件的输入流类,以读取方式打开名为 file_name 的文件
        std::ifstream fin(file_name);
        if (!fin) {
            std::cerr << "file not found: " << file_name << "'" << std::endl;
            std::exit(1);
        }

        int n = 0, m = 0; // n表示cnf中变量数目，m表示cnf中子句数目
        // 逐字符读取文件内容，直到达到文件末尾或解析完cnf前两行
        while (!fin.eof()) {
            char ch;
            fin >> ch;                  // 读取到的每个字符都保存到ch中

            if (ch == 'c') {            // 字符'c'表示注释
                char buf[1024];
                fin.getline(buf, 1024); // 读取注释并跳过注释行
            } else if (ch == 'p') {     // 字符'p'表示一行指示信息，下一行开始为子句clause
                std::string buf;
                fin >> buf;             // 读取下一个字符串
                assert(buf == "cnf");   // 判断是否合法，如果为假，那么先向stderr打印一条出错信息，然后通过调用 abort 来终止程序运行。
                fin >> n >> m;          // n表示cnf中变量数目，m表示cnf中子句数目
                break;
            } else {                    // unexpected line
                std::cerr << "parse error at char '" << ch << "'" << std::endl;
                std::exit(1);
            }
        }

        // clauses begin 开始解析子句
        // 解析的子句存储在 std::vector<clause> clauses中，并返回一个包含完整 CNF 表达式的 formula 对象
        std::vector<clause> clauses;
        for (int i = 0; i < m; i++) {
            clause c; // clause表示文字的析取，类型为vector<literal>，literal表示文字，int 类型
            while (!fin.eof()) {
                int v;
                fin >> v;
                if (v == 0) {
                    // 内部循环在读取到值为0的变量时终止，表示子句的结束
                    clauses.push_back(c);
                    break;
                }
                assert(VAR(v) <= n); // 确保变量的值不超过 n，即不超过预期的变量数量；VAR给变量取绝对值
                c.push_back(v);      // 如果v不为0，则将其添加到当前子句c中，并继续循环
            }
        }
        assert(clauses.size() == m); // 使用 assert 断言确保实际解析的子句数量等于预期的子句数量

        // 创建一个 formula 对象，将解析得到的变量数量 n 和子句向量 clauses 作为参数传递给构造函数，并将该对象返回
        return formula(n, clauses);
    }
};

#endif // DPLL_DIMACSPARSER_H
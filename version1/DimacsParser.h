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

        int literal_num = 0, clause_num = 0;
        // 逐字符读取文件内容，直到达到文件末尾或解析完cnf的注释行(begin with c)和说明行(begin with p)
        while (!fin.eof()) {
            char ch;
            fin >> ch;                            // 读取到的每个字符都保存到ch中

            if (ch == 'c') {                      // 字符'c'表示注释
                char buf[1024];
                fin.getline(buf, 1024);           // 读取注释并跳过注释行
            } else if (ch == 'p') {               // 字符'p'表示一行指示信息，下一行开始为子句clause，读取完直接break
                std::string buf;
                fin >> buf;                       // 读取下一个字符串
                assert(buf == "cnf");             // 确保在'p'后读取到的字符串为"cnf"
                fin >> literal_num >> clause_num; // n表示cnf中变量数目，m表示cnf中子句数目
                break;
            } else {                              // unexpected line
                std::cerr << "parse error at char '" << ch << "'" << std::endl;
                std::exit(1);
            }
        }

        // clauses begin 开始解析clause
        // clause 存储在 std::vector<clause> clauses 中，并返回一个包含完整 CNF formuila 的 formula 对象
        std::vector<clause> clauses;
        for (int i = 0; i < clause_num; i++) {
            clause c; // clause 表示文字的析取，typedef vector<literal> clause，using literal = int;
            while (!fin.eof()) {
                int v;
                fin >> v;
                if (v == 0) {
                    // 内部循环在读取到值为0的变量时终止，0 is the end of a clause
                    clauses.push_back(c);
                    break;
                }
                assert(VAR(v) <= literal_num); // VAR给变量取绝对值,确保 literal 不超过 n，即不超过预期的变量数量；
                c.push_back(v);                // 合法的 literal,加入到clause中
            }
        }
        assert(clauses.size() == clause_num); // assert 断言确保实际解析的子句数量等于预期的子句数量

        // 创建一个 formula 对象，将解析得到的 literal_num 和 clauses 作为参数传递给构造函数，并返回该formula对象
        return formula(literal_num, clauses);
    }
};

#endif // DPLL_DIMACSPARSER_H
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include "common.h"
#include <algorithm> // for sort

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
        std::ifstream fin(file_name); // open the cnf file by ifstream obj.
        if (!fin) {
            std::cerr << "file not found: " << file_name << "'" << std::endl;
            std::exit(1);
        }

        int literal_num = 0, clause_num = 0;
        // parse the file until the 'p' paragraph
        while (!fin.eof()) {
            char ch;
            fin >> ch;       // save every char in the ch

            if (ch == 'c') { // skip the comment paragraph
                char buf[1024];
                fin.getline(buf, 1024);
            } else if (ch == 'p') {   // parse literal_num&clause_num in this paragraph
                std::string buf;
                fin >> buf;           // read the next string
                assert(buf == "cnf"); // make sure the next string is "cnf"
                fin >> literal_num >> clause_num;
                break;                // the next paragraph is for clause
            } else {                  // unexpected line
                std::cerr << "parse error at char '" << ch << "'" << std::endl;
                std::exit(1);
            }
        }

        // start parsing clause,store clause in std::vector<clause> clauses,and return formula class.
        clauses_t clauses;
        std::unordered_map<int, int> freqCount;
        for (int i = 0; i < clause_num; i++) {
            clause_t c; // typedef vector<literal> clause，using literal = int;
            while (!fin.eof()) {
                int v;
                fin >> v;
                if (v == 0) {
                    // 0 is the end of a clause,continue to parse the next clause then.
                    clauses.push_back(c);
                    break;
                }
                assert(VAR(v) <= literal_num); // make sure the literal <= literal_num
                freqCount[v]++;                // Statistical variable frequency
                c.insert(v);                   // insert the leagal literal in this clause
            }
        }
        assert(clauses.size() == static_cast<size_t>(clause_num)); // assert 确保实际解析的子句数量等于预期的子句数量
        // sort freq by value
        // 将键值对存储到 vector 中
        std::vector<std::pair<int, int>> countVector(freqCount.begin(), freqCount.end());

        // 对 vector 按照出现次数降序排序
        std::sort(countVector.begin(), countVector.end(), [](const auto &a, const auto &b) {
            return a.second > b.second;
        });

        return formula(literal_num, clause_num, clauses, countVector);
    }

    static clauses_t getClauses(formula &CNF_formula) {
        return CNF_formula.clauses;
    }

    static void showClauses(formula &CNF_formula) {
        std::cout << "===========  show CNF begin  ============" << std::endl;
        std::cout << "CNF formula :"
                  << "nbvar: " << CNF_formula.num_variables << ","
                  << "nbclauses: " << CNF_formula.num_clauses << std::endl;

        for (auto clause : CNF_formula.clauses) {
            for (auto literal : clause) {
                std::cout << literal << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << "============  show CNF end   ============" << std::endl;
    }

    static void showFreq(formula &CNF_formula) {
        std::cout << "===========  show literal frequency  ============" << std::endl;

        for (auto literal_freq : CNF_formula.freq) {
            std::cout << literal_freq.first << ": " << literal_freq.second << std::endl;
        }
        std::cout << "============  show literal end   ============" << std::endl;
    }
};

#endif // DPLL_DIMACSPARSER_H
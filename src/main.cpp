#include <chrono>
#include <string>
#include <cstring>
#include "../include/common.h"
#include "../include/DimacsParser.h"
#include "../include/DPLLsolver.h"
#include "../include/CDCLsolver.h"

#define PRINT 0

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "error: no input files" << std::endl;
        printf("usage：./SatSolver <xx.cnf> <mode> <isPrint>\n");
        return 1;
    }

    std::string f(argv[1]);

    // std::string f="test/test1.cnf";
    std::cout << f << std::endl;

    auto start = std::chrono::steady_clock::now();
    formula cnf_formula = DimacsParser::parse(f);
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
    std::cout << "parse cnf file successed,time:" << duration.count() << std::endl;

    // std::cout << "literal num:" << cnf_formula.num_variables << "\tclauses num:" << cnf_formula.num_clauses << std::endl;
    if (!strcmp(argv[3], "1")) DimacsParser::showClauses(cnf_formula);
    if (!strcmp(argv[3], "1")) DimacsParser::showFreq(cnf_formula);

    bool sat;
    // mode: 1->dpll&cdcl 2->dpll 3->cdcl
    if (!strcmp(argv[2], "1")) {
        std::cout << "\nstart solving by DPLL algorithm\n";
        // DPLL timer start
        start = std::chrono::steady_clock::now();
        DPLLSolver solver(cnf_formula);
        sat = solver.solve(cnf_formula.clauses, std::set<int>());
        model m;
        if (sat) {
            m = solver.get_model();
        }
        end = std::chrono::steady_clock::now();
        // timer end

        if (sat) {
            std::cout << "sat" << std::endl;
            if (!strcmp(argv[3], "1")) {
                std::cout << "=========== show result begin ===========" << std::endl;
                for (const auto &p : m) {
                    std::cout << p.first << " -> " << p.second << std::endl;
                }
                std::cout << "============ show result end ============" << std::endl;
            }
        } else {
            std::cout << "unsat" << std::endl;
        }

        duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        std::cout << "time: " << duration.count() << std::endl;

        // CDCL start
        std::cout << "\nstart solving by CDCL algorithm\n";
        start = std::chrono::steady_clock::now();
        CDCLSolver cdclSoler(cnf_formula);
        sat = cdclSoler.solve();
        end = std::chrono::steady_clock::now();
        if (sat) {
            std::cout << "sat" << std::endl;
            if (!strcmp(argv[3], "1")) {
                cdclSoler.printResult(std::cout);
            }
        } else {
            std::cout << "unsat" << std::endl;
        }
        duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        std::cout << "time: " << duration.count() << std::endl;

        if (sat)
            return 0;
        else
            return 1;
    } else if (!strcmp(argv[2], "2")) {
        // dpll only
        // DPLL timer start
        std::cout << "\nstart solving by DPLL algorithm\n";
        start = std::chrono::steady_clock::now();
        DPLLSolver solver(cnf_formula);
        sat = solver.solve(cnf_formula.clauses, std::set<int>());
        model m;
        if (sat) {
            m = solver.get_model();
        }
        end = std::chrono::steady_clock::now();
        // timer end

        if (sat) {
            std::cout << "sat" << std::endl;
            if (!strcmp(argv[3], "1")) {
                std::cout << "=========== show result begin ===========" << std::endl;
                for (const auto &p : m) {
                    std::cout << p.first << " -> " << p.second << std::endl;
                }
                std::cout << "============ show result end ============" << std::endl;
            }

        } else {
            std::cout << "unsat" << std::endl;
        }

        duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        std::cout << "time: " << duration.count() << std::endl;
        if (sat)
            return 0;
        else
            return 1;
    } else if (!strcmp(argv[2], "3")) {
        // cdcl only
        // CDCL start
        std::cout << "\nstart solving by CDCL algorithm\n";
        start = std::chrono::steady_clock::now();
        CDCLSolver cdclSoler(cnf_formula);
        sat = cdclSoler.solve();
        end = std::chrono::steady_clock::now();
        if (sat) {
            std::cout << "sat" << std::endl;
            if (!strcmp(argv[3], "1")) {
                cdclSoler.printResult(std::cout);
            }

        } else {
            std::cout << "unsat" << std::endl;
        }
        duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        std::cout << "time: " << duration.count() << std::endl;
    }
    if (sat)
        return 0;
    else
        return 1;
}
#include <chrono>

#include "common.h"
#include "DimacsParser.h"
#include "DPLL.h"

#define PRINT 0

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "error: no input files" << std::endl;
        printf("usage：./SatSolver xx.cnf\n");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        std::string f(argv[i]);
        // std::string f="test/test1.cnf";
        std::cout << f << std::endl;
        auto start = std::chrono::steady_clock::now();
        formula cnf_formula = DimacsParser::parse(f);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        std::cout << "parse cnf file successed,time:" << duration.count() << " literal nums is:" << cnf_formula.num_variable << std::endl;
        std::cout << "start solving\n";
        // timer start
        start = std::chrono::steady_clock::now();
        DPLL solver(cnf_formula);
        bool sat = solver.solve();
        model m;
        if (sat) {
            m = solver.get_model();
        }
        end = std::chrono::steady_clock::now();
        // timer end

        if (sat) {
            std::cout << "  sat" << std::endl;
            if (PRINT) {
                for (const auto &p : m) {
                    std::cout << "    " << p.first << " -> " << p.second << std::endl;
                }
            }
        } else {
            std::cout << "  unsat" << std::endl;
        }

        duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        std::cout << "  time: " << duration.count() << std::endl;
    }

    return 0;
}
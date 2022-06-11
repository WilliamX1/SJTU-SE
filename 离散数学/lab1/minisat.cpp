#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <zlib.h>
#include <iostream>
#include "minisat/utils/System.h"
#include "minisat/utils/ParseUtils.h"
#include "minisat/utils/Options.h"
#include "minisat/core/Dimacs.h"
#include "minisat/core/Solver.h"

using namespace Minisat;

int main()
{
    Solver solver;
    auto A = solver.newVar();
    auto B = solver.newVar();
    auto C = solver.newVar();

    // Create the clauses
    solver.addClause( ~mkLit(A),  ~mkLit(B),  mkLit(C));
    solver.addClause(~mkLit(A),  ~mkLit(B),  ~mkLit(C));
    solver.addClause( mkLit(A), ~mkLit(B),  mkLit(C));


    // Check for solution and retrieve model if found
    auto sat = solver.solve();
    if (sat) {
        std::clog << "SAT\n"
                  << "Model found:\n";
        std::clog << "A := " << (solver.modelValue(A) == l_True) << '\n';
        std::clog << "B := " << (solver.modelValue(B) == l_True) << '\n';
        std::clog << "C := " << (solver.modelValue(C) == l_True) << '\n';
    } else {
        std::clog << "UNSAT\n";
        return 1;
    }
}




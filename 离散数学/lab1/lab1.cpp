#include "common.h"

using namespace Minisat;

bool lab1(int states[], int n, int button[][2], int m, bool answer[])
{
    Solver solver;
    Var variables[1000];
    int i, j;
    for(i = 0; i < m; ++i)
        variables[i] = solver.newVar();

    //TODO : your solution here.
    int rock[n + 1][2];
    for (i = 1; i <= n; i++) rock[i][0] = rock[i][1] = -1;

    for (int i = 0; i < m; i++)
    {
        if (rock[button[i][0]][0] == -1)
            rock[button[i][0]][0] = i;
        else rock[button[i][0]][1] = i;

        if (button[i][1])
        {
            if (rock[button[i][1]][0] == -1)
                rock[button[i][1]][0] = i;
            else rock[button[i][1]][1] = i;
        }
    }

    //for (int i = 1; i <= n; i++)
        //printf("%d %d\n", rock[i][0], rock[i][1]);
    //for (int i = 1; i <= n; i++)
        //printf("%d %d %d %d\n", states[i], rock[i][0], rock[i][1], variables[rock[i][0]] ^ variables[rock[i][1]]);
    //printf("\n");
    
    for (int i = 1; i <= n; i++)
    {
        int A = rock[i][0], B = rock[i][1];
        if (B == -1)
            if (states[i - 1])
                solver.addClause(~mkLit(variables[A]));
            else solver.addClause(mkLit(variables[A]));
        else if (B != -1)
        // A ^ B = (A | B) & (~A | ~B);
        //~(A ^ B) = (~A | B) & (A | ~B);
	        if (states[i - 1]){
		        solver.addClause(~mkLit(variables[A]), mkLit(variables[B]));
                solver.addClause(mkLit(variables[A]), ~mkLit(variables[B]));
            }
            else{
                solver.addClause(mkLit(variables[A]), mkLit(variables[B]));
		        solver.addClause(~mkLit(variables[A]), ~mkLit(variables[B]));
            }

    }
            
    //solve SAT problem
    auto sat = solver.solve();
    if(sat)
    {
        for(i = 0; i < m; ++i)
            answer[i] = (solver.modelValue(variables[i]) == l_True);
        return true;
    }
    return false; 
}





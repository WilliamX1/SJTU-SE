#include <stdio.h>
#include <z3++.h>
#include <iostream>
using namespace z3;


int main()
{
    context ctx;

    expr x = ctx.int_const("x");
    expr y = ctx.int_const("y");
    expr z = ctx.int_const("z");
    solver s(ctx);

    s.add(x==1 || x==3);
    s.add(y==1 || y==2 || y==3);
    s.add(z==3);
    s.add(x != y);
    s.add(x != z);
    s.add(y != z);

    if(s.check() == sat) {
        printf("sat\n");

        if(sat == s.check()){
            model m = s.get_model();
            printf("x = %d\n",  m.eval(x).get_numeral_int());
            printf("y = %d\n",  m.eval(y).get_numeral_int());
            printf("z = %d\n",  m.eval(z).get_numeral_int());
        }

    } else {
        printf("unsat\n");
    }

}



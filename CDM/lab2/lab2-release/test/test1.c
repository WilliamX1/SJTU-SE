int test1(int x, int y) {
    int a;
    
    a = ~((~(~x&y))&(~(x&~y)));
    assert(a != (x^y) );
}

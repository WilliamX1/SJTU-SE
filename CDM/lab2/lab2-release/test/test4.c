int test4(int x) {
    int a, b;
    
    a = (~x+1)&x;
    b = 0;
    assert(a == b);

}
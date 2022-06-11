int test5(int x) {
    
    int t1, t2, a;
    t1 = (x + (~0x30 + 1)) >> 31;
    t2 = (0x39 + (~x + 1)) >> 31;

    assert( (t1 | t2) + 1 == 1 );
} 
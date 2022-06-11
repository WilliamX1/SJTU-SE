int test6(int x,int y) {
    int t1, t2, t, cf, a;
    
    t1 = x >> 31;
    t2 = y >> 31;
    t = t1 ^ t2;
    cf = (x + (~y + 1)) >> 31;
    

    a = x - y;
    
    assert( 1 == (t & (t1 ^ cf)) + 1 );
}
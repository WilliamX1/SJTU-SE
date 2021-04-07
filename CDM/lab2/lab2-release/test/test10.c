int test10(int x, int y) {
    int t1, t2, t;
    t1 = x >> 31;
    t2 = y >> 31;
    t = (x + (~y + 1)) >> 31;
    
    if(x<y) {
        assert( 1 != ((~(~(t1 ^ t2) & t) + 1) | (~(t1 & ~t2) + 1) ) );
    }
    else {
        assert( 0 != ((~(~(t1 ^ t2) & t) + 1) | (~(t1 & ~t2) + 1) ) );
    }
}
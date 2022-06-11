int test9(int a, int n, int m) {
    

    int t1, t2, x, c, d, cc;
    
    x = a;
    
    if(n>=0 && n<=3 && m>=0 && m<=3) {
        t1 = (x >> (n << 3)) & 0xFF;
        t2 = (x >> (m << 3)) & 0xFF;
        x = (x & (~ (0xFF << (m << 3)))) | (t1 << (m << 3));
        x = (x & (~ (0xFF << (n << 3)))) | (t2 << (n << 3));
        
        d = x;
        
        x = a;
        c=m+(~n+1);
        cc = c >> 31;
        c = (c ^ cc) - cc;
        x = x ^ ((x << (c << 3)) & (0xFF << (m << 3)));
        x = x ^ ((x >> (c << 3)) & (0xFF << (n << 3)));
        x = x ^ ((x << (c << 3)) & (0xFF << (m << 3)));
        
        assert(x == d);
    }

    return 0;
}
int test8(int m) {
  int x;
  
  x = m;
  x = x | (x << 16);
  x = x | (x << 8);
  x = x | (x << 4);
  x = x | (x << 2);
  x = x | (x << 1);

  assert( 1 == (x >> 31) + 1 );
}
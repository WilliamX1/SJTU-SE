method method2(a: array<int>, v: int) returns (b: int)
// Add a precondition here.
   requires 0 < v && v < a.Length
{
   return a[v] / v;
}

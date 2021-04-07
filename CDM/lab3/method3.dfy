predicate notzero(a: array<int>)
  reads a
{
// Add a predicate here.
  forall i :: 0 <= i < a.Length ==> a[i] != 0 
}

method method3(a : array<int>, n : int) returns (b : int)
  requires n == a.Length && notzero(a)
  ensures b == 0;
{
  var i := 0;
  while i < n
      invariant 0 <= i <= a.Length
      invariant n == a.Length
      invariant forall k :: 0 <= k < i ==> a[k] != 0
  {
    if a[i] == 0
    { return 1; }
   
    i := i + 1;
  }
  return 0;
}

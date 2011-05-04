
int fibo(int n) {
  if(n < 3) return 1;
  return fibo(n-1) + fibo(n-2);
}

print "fibo(36):", fibo(36);


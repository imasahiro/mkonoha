#define is_const(expr) __builtin_constant_p(expr)
#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#define prefetch(x) __builtin_prefetch(x)
//void __builtin_prefetch( const void *addr, int rw, int locality );
//

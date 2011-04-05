
#ifndef KNH_T
#define KNH_T(T) knh_##T##_t
#endif

#define Tuple(K, V) knh_Tuple_##K##_##V##_t

#define DEF_TUPLE_STRUCT(K, V) \
struct Tuple(K, V) {\
    knh_hObject_t h;\
    enum object_type type1;\
    enum object_type type2;\
    KNH_T(K) *o1;\
    KNH_T(V) *o2;\
}

#define DEF_TUPLE_T(K, V)               \
struct Tuple(K, V);                     \
typedef struct Tuple(K, V) Tuple(K, V)

DEF_TUPLE_T(void, void);
DEF_TUPLE_STRUCT(void, void);
typedef Tuple(void, void) knh_Tuple_t;

#define tuple_op(o, op, idx) (assert(tuple_##op((knh_Tuple_t*)o, idx)))
static inline int tuple_check_type1(knh_Tuple_t *a, knh_class_t type)
{
    return (a->type1 == type);
}
static inline int tuple_check_type2(knh_Tuple_t *a, knh_class_t type)
{
    return (a->type2 == type);
}

#define DEF_TUPLE_OP(K, V)\
static Tuple(K, V) *Tuple_new_##K##_##V (KNH_T(K) *k, KNH_T(V) *v) {    \
    Tuple(K, V) *t = cast(Tuple(K, V) *, new_Tuple());                  \
    t->type1 = TYPE_##K;                                                \
    t->type2 = TYPE_##V;                                                \
    t->o1    = k;                                                       \
    t->o2    = v;                                                       \
    return t;                                                           \
}\
static KNH_T(K) *Tuple_##K##_##V##_K(Tuple(K, V) *t) {\
    return t->o1;                                     \
}\
static KNH_T(V) *Tuple_##K##_##V##_V(Tuple(K, V) *t) {\
    return t->o2;                                     \
}\
static void Tuple_##K##_##V##_setK(Tuple(K, V) *t, KNH_T(K) *v){ \
    tuple_op(t, check_type1, O(v)->h.classinfo);                 \
    t->o1 = v;                                                  \
}\
static void Tuple_##K##_##V##_setV(Tuple(K, V) *t, KNH_T(V) *v){ \
    tuple_op(t, check_type2, O(v)->h.classinfo);                 \
    t->o2 = v;                                                  \
}

#define Tuple_K(K, V, o)       (KNH_T(T) *) Tuple_##K##_##V##_K(o, v)
#define Tuple_V(K, V, o)       (KNH_T(V) *) Tuple_##K##_##V##_V(o, v)
#define Tuple_setK(K, V, o, v)              Tuple_##K##_##V##_setK(o, v)
#define Tuple_setV(K, V, o, v)              Tuple_##K##_##V##_setV(o, v)
#define Tuple_new(K, V)                     Tuple_new_##K##_##V (NULL, NULL)
#define Tuple_new_init(K, V, k, v)          Tuple_new_##K##_##V (k, v)

knh_Tuple_t *new_Tuple(void);
#define T(t) ((knh_Tuple_t*)(t))


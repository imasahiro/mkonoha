#ifndef ARRAY_H
#define ARRAY_H

#define Array(T) knh_Array_##T##_t

//##define @type Array
//struct knh_Array_t {
//    knh_hObject_t h;
//    enum object_type type;
//    int size;
//    int capacity;
//    void **list;
//};

#define DEF_ARRAY_STRUCT(T) \
struct Array(T) {\
    knh_hObject_t h;\
    enum object_type type;\
    int size;  \
    int capacity;  \
    knh_##T##_t **list;\
}
#define DEF_ARRAY_T(T)              \
struct Array(T);                    \
typedef struct Array(T) Array(T);   \

DEF_ARRAY_T(void);
DEF_ARRAY_STRUCT(void);
typedef Array(void) knh_Array_t;

#define array_op(o, op, idx) (assert(array_##op((knh_Array_t*)o, idx)))
static inline int array_check_index(knh_Array_t *a, int idx)
{
    return (idx < a->size);
}
static inline int array_check_type(knh_Array_t *a, knh_class_t type)
{
    return (a->type == type);
}

#define DEF_ARRAY_OP(T)\
static inline Array(T) *Array_new_##T (void) {             \
    Array(T) *a = cast(Array(T) *, new_Array());    \
    a->type = TYPE_##T;                             \
    return a;                                       \
}\
static inline void Array_##T##_add(Array(T) *a, KNH_T(T) *v) {\
    array_op(a, check_type, O(v)->h.classinfo);     \
    if (a->size + 1 >= a->capacity) {\
        a->list = (KNH_T(T)**)realloc(a->list, sizeof(KNH_T(T)*) * a->capacity * 2);\
        a->capacity *= 2;\
    }\
    a->list[a->size++] = v;                         \
}\
static inline KNH_T(T) *Array_##T##_get(Array(T) *a, int idx) {\
    array_op(a, check_index, idx);                  \
    return a->list[idx];                            \
}\
static inline void Array_##T##_set(Array(T) *a, int idx, KNH_T(T) *v){ \
    array_op(a, check_index, idx);                  \
    array_op(a, check_type, O(v)->h.classinfo);     \
    a->list[idx] = v;                               \
}\
static inline void Array_##T##_delete(Array(T) *a) {\
    free(a);                                        \
}

#define DEF_ARRAY_S_STRUCT(T) \
struct Array(T) {\
    knh_hObject_t h;\
    enum object_type type;\
    int size;  \
    int capacity;  \
    knh_##T##_t *list;\
}
#define DEF_ARRAY_S_T(T) DEF_ARRAY_T(T)

#define DEF_ARRAY_S_OP_NOEQ(T)\
static inline Array(T) *Array_new_##T (void) {      \
    Array(T) *a = cast(Array(T) *, new_Array());    \
    return a;                                       \
}\
static inline void Array_##T##_add(Array(T) *a, KNH_T(T) v) {\
    if (a->size + 1 >= a->capacity) {                \
        a->capacity *= 2;\
        a->list = (KNH_T(T)*)realloc(a->list, sizeof(KNH_T(T)) * a->capacity);\
    }\
    a->list[a->size++] = v;                         \
}\
static inline KNH_T(T) Array_##T##_get(Array(T) *a, int idx) {\
    array_op(a, check_index, idx);                  \
    return a->list[idx];                            \
}\
static inline void Array_##T##_set(Array(T) *a, int idx, KNH_T(T) v){ \
    array_op(a, check_index, idx);                  \
    a->list[idx] = v;                               \
}\
static inline void Array_##T##_delete(Array(T) *a) {\
    free(a);                                        \
}\

#define DEF_ARRAY_S_OP(T)\
DEF_ARRAY_S_OP_NOEQ(T);\
static inline int Array_##T##_eq(Array(T) *a1, Array(T) *a2){ \
    int i; KNH_T(T) x;                      \
    if (Array_size(a1) == Array_size(a2)) return 0;\
    FOR_EACH_ARRAY(a1, x, i) {              \
        if (x != Array_n(a2, i)) return 0;  \
    }\
    return 1;\
}

#define Array_get(T, a, idx)    Array_##T##_get(a, idx)
#define Array_set(T, a, idx, v) Array_##T##_set(a, idx, v)
#define Array_add(T, a, v)      Array_##T##_add(a, v)
#define Array_delete(T, a)      Array_##T##_delete(a)
#define Array_new(T)            Array_new_##T ()
#define Array_eq(T, a1, a2)     Array_##T##_eq(a1, a2)
#define Array_n(a, n) (a->list[n])
#define Array_size(a) (a->size)
#define Array_last(a) (Array_n(a, Array_size(a) - 1))
knh_Array_t *new_Array(void);

#define FOR_EACH_ARRAY(a, x, i) \
    for(i=0, x = a->list[i]; i < (a)->size; x=a->list[(++i)])
#define FOR_EACH_ARRAY_INIT(a, x, i, _init_) \
    for(_init_, x = a->list[i]; i < (a)->size; x=a->list[(++i)])

#endif /* end of include guard: ARRAY_H */

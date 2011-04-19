#ifndef MEMORY_H
#define MEMORY_H

#define malloc_(len) (malloc(len))
#define new_(T)      (knh_##T##_t *) __new(sizeof(knh_##T##_t), TYPE_##T)
#define realloc_(ptr, len) (realloc(ptr, len))
#define free_(ptr) { \
    free(ptr); \
    ptr = NULL;\
}
#define delete_(ptr) free_(ptr)
#define copy_(T, v, size) ((T) __copy((void*)v, size))
#define copy_1(T, v) copy_(T, v, sizeof(T))


#endif /* end of include guard: MEMORY_H */

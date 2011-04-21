#ifndef OBJECT_H
#define OBJECT_H

typedef intptr_t knh_class_t;
typedef void knh_void_t;
typedef int64_t knh_data_t;

typedef struct knh_string_t {
    int  len;
    char txt[];
} knh_string_t;

struct index_t {
    short i;
    short x;
};

typedef union knh_value {
    knh_data_t    dval;
    knh_int_t     ival;
    knh_float_t   fval;
    void         *ptr;
    knh_string_t *str;
    knh_Object_t *o;
    struct index_t index;
    struct knh_Method_t *mtd;
    struct knh_ObjectField_t *ox;
} knh_value_t;


#define MINIKONOHA_MAGIC (0xcafebebe)

typedef struct knh_hObject_t {
    uintptr_t magicflag;
    union {
        void *name;
        knh_class_t classinfo;
    };
    void *    meta;
} knh_hObject_t;

struct knh_Object_t {
    knh_hObject_t h;
    void *ref1;
    void *ref2;
    void *ref3;
    void *ref4;
};

struct knh_ObjectField_t {
    knh_hObject_t h;
    knh_value_t *ox;
    knh_value_t unused1;
    knh_value_t unused2;
    knh_value_t unused3;
};

enum object_type {
    TYPE_Object,
    TYPE_Integer,
    TYPE_Float,
    TYPE_String,
    TYPE_Token,
    TYPE_Class,
    TYPE_Method,
    TYPE_Array,
    TYPE_Tuple,
    TYPE_OutputStream,
    TYPE_InputStream,
    TYPE_MAX,
    TYPE_UNTYPED = -1,
    TYPE_void    = -2
};

#define O(o) ((knh_Object_t*)o)

#endif /* end of include guard: OBJECT_H */

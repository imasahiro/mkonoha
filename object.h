typedef intptr_t knh_class_t;
typedef void knh_void_t;

typedef struct knh_string_t {
    int  len;
    char txt[];
} knh_string_t;

typedef union knh_value {
    void         *ptr;
    int           ival;
    double        fval;
    knh_string_t *str;
    knh_Object_t *o;
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
    TYPE_MAX,
    TYPE_UNTYPED = -1,
    TYPE_void    = -2
};

#define O(o) ((knh_Object_t*)o)

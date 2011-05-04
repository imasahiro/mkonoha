#ifndef K_STRING_H
#define K_STRING_H

knh_string_t *new_string(const char *txt);
#define string_length(s) ((s)->len)
#define string_str(s) ((s)->txt)
#define string_cmp(s1, s2) strncmp(string_str(s1), string_str(s2), string_length(s1))
#define string_cmp_char(s1, txt, len) (strncmp(string_str(s1), txt, len))
typedef unsigned char knh_bool_t;
knh_int_t String_length(knh_string_t *self);
knh_string_t* String_concat1(knh_string_t *self, knh_string_t* arg1);
knh_string_t* String_opPlus(knh_string_t* self, knh_string_t* arg1);
knh_bool_t String_eq(knh_string_t* self, knh_string_t* arg1);
knh_bool_t String_neq(knh_string_t* self, knh_string_t* arg1);


#endif /* end of include guard: K_STRING_H */

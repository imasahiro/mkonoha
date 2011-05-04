
/**** String ****/
/* Int String.length
 * :propaty
 * # get length of string
 */
knh_int_t String_length(knh_string_t *self)
{
    return string_length(self);
}

/* String String.concat1(String arg1)
 * :method
 * # concat values
 */
knh_string_t* String_concat1(knh_string_t *self, knh_string_t* arg1)
{
    int len0 = string_length(self);
    int len1 = string_length(arg1);
    int len = len0 + len1 - 1;
    knh_string_t *ret = malloc_(sizeof(knh_string_t) + len);
    memcpy(ret->txt, self->txt, len0);
    memcpy(ret->txt + len0 - 1, arg1->txt, len1);
    ret->txt[len] = '\0';
    ret->len = len;
    return ret;
}

knh_string_t* String_opPlus(knh_string_t* self, knh_string_t* arg1)
{
    int len0 = string_length(self);
    int len1 = string_length(arg1);
    int len = len0 + len1 - 1;
    knh_string_t *ret = malloc_(sizeof(knh_string_t) + len);
    ret->len = len;
    memcpy(ret->txt, self->txt, len0);
    memcpy(ret->txt + len0 - 1, arg1->txt, len1);
    return ret;
}

knh_bool_t String_eq(knh_string_t* self, knh_string_t* arg1)
{
    int len0 = string_length(self);
    int len1 = string_length(arg1);
    return (len0 == len1 && string_cmp(self, arg1) == 0);
}

knh_bool_t String_neq(knh_string_t* self, knh_string_t* arg1)
{
    int len0 = string_length(self);
    int len1 = string_length(arg1);
    return (len0 != len1 && string_cmp(self, arg1) != 0);
}



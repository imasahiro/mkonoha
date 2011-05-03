#ifndef STREAM_H
#define STREAM_H

struct vm;
struct io;
struct knh_OutputStream_t;
static struct io *new_io(const char *name, const char *mode, int iotype);
static void io_puts(struct io *io, const char *cbuf, int len);
static void io_gets(struct io *io, char *cbuf, int len);
static void io_printf(struct io *io, int nargs, const char *fmt, ...);
static void io_delete(struct io *io);
static void stream_init(void);

struct knh_OutputStream_t;
struct knh_OutputStream_t *new_OutputStream(struct knh_string_t *name, struct knh_string_t *mode);
void OutputStream_delete(struct knh_OutputStream_t *os);
void OutputStream_print(struct knh_OutputStream_t *os, knh_class_t cid, knh_value_t v);
void OutputStream_println(struct knh_OutputStream_t *os, knh_class_t cid, knh_value_t v);


void knh_OutputStream_print(struct vm *vm);

#define STREAM(s) (type_check_stream(O(s)))
static inline struct knh_OutputStream_t *type_check_stream(struct knh_Object_t *o)
{
    if (o->h.classinfo == TYPE_InputStream || o->h.classinfo == TYPE_OutputStream) {
        return cast(struct knh_OutputStream_t *,o);
    }
    return NULL;
}
struct io *IOStream_getIO(struct knh_OutputStream_t *s);

#endif /* end of include guard: STREAM_H */

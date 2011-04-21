#include "konoha.h"
#include "memory.h"
#include "stream.h"

struct io;
struct iobuf {
    int size, pos;
    unsigned char *buf;
};

struct iodrv {
    int bufsize;
    int           (*read_) (struct io *io, struct iobuf *buf);
    int           (*write_)(struct io *io, struct iobuf *buf);
    void          (*close_)(struct io *io);
    void          (*sync_) (struct io *io);
    struct io    *(*open_) (const char *name, const char *mode, int bufsize);
};

struct io {
    void    *data_;
    int    (*read_) (struct io *io, struct iobuf *buf);
    int    (*write_)(struct io *io, struct iobuf *buf);
    void   (*close_)(struct io *io);
    void   (*sync_) (struct io *io);
    char is_r, is_w;
    struct iobuf buf;
};

typedef struct iodrv *knh_iodrv_t;
DEF_ARRAY_S_STRUCT(iodrv);
DEF_ARRAY_S_T(iodrv);
DEF_ARRAY_S_OP(iodrv);
Array(iodrv) *g_iodrv = NULL;

static struct io *new_io(const char *name, const char *mode, int iotype)
{
    struct io *io;
    struct iodrv  *drv = Array_get(iodrv, g_iodrv, iotype);
    io = drv->open_(name, mode, drv->bufsize);
    return io;
}

#define FILE_(io)  ((FILE*)(io)->data_)
#define FILE_BUF_SIZE 1024
struct fileblock {
    unsigned char b[FILE_BUF_SIZE];
};

static int FILE_read_ (struct io *io, struct iobuf *buf)
{
    FILE *fp = FILE_(io);
    struct iobuf *iobuf = &io->buf;
    int size = buf->size, bsize = iobuf->size;
    if (iobuf->pos + size < bsize) {
        memcpy(buf->buf, iobuf->buf + iobuf->pos, size);
        iobuf->pos += size;
        return size;
    }

    {
        int n = 0, segments = size / FILE_BUF_SIZE;
        struct fileblock *fblk = (struct fileblock*) buf->buf;
        while (segments > 0) {
            struct fileblock *base;
            size_t len;

            base = fblk;
            len  = FILE_BUF_SIZE;
            fblk++;
            segments--;
            n = fread(base, len, 1, fp);
            size += n;
        }
        fread(iobuf->buf, FILE_BUF_SIZE, 1, fp);
        memcpy(fblk, iobuf->buf, size % FILE_BUF_SIZE);
        iobuf->pos = size % FILE_BUF_SIZE;
        iobuf->size = FILE_BUF_SIZE;
    }
    return size;
}

static int FILE_write_(struct io *io, struct iobuf *buf)
{
    FILE *fp = FILE_(io);
    struct iobuf *iobuf = &io->buf;
    int size = buf->size, bsize = iobuf->size;
    struct fileblock *fblk = (struct fileblock*) buf->buf;
    /* fast path */
    if (iobuf->pos + size < bsize) {
        unsigned char *cbuf = iobuf->buf + iobuf->pos;
        memcpy(cbuf, fblk, size);
        iobuf->pos += size;
        return size;
    }

    {
        int n = 0, segments = size / FILE_BUF_SIZE;
        if (iobuf->pos && iobuf->size - iobuf->pos != FILE_BUF_SIZE) {
            n = fwrite(iobuf->buf + iobuf->pos, iobuf->size - iobuf->pos, 1, fp);
        }
        while (segments > 0) {
            struct fileblock *base;
            size_t len;

            base = fblk;
            len  = FILE_BUF_SIZE;
            fblk++;
            segments--;
            n += fwrite(base, len, 1, fp);
        }
        memcpy(iobuf->buf, fblk, size % FILE_BUF_SIZE);
        iobuf->pos = size % FILE_BUF_SIZE;
        iobuf->size = FILE_BUF_SIZE;
    }
    return size;
}

static void FILE_close_(struct io *io)
{
    FILE *fp = FILE_(io);
    fclose(fp);
}

static void FILE_sync_ (struct io *io)
{
    FILE *fp = FILE_(io);
    struct iobuf *iobuf = &io->buf;
    if (io->is_w && iobuf->pos > 0) {
        fwrite(iobuf->buf, iobuf->pos, 1, fp);
        iobuf->pos = 0;
    }
}

#define IODRV_SET(io, r, w, c, s) {\
    if (r != NULL) {io->read_  = r;} else {}\
    if (w != NULL) {io->write_ = w;} else {}\
    if (c != NULL) {io->close_ = c;} else {}\
    if (s != NULL) {io->sync_  = s;} else {}\
}
#define NEW(T) ((T*)malloc_(sizeof(T)))
static struct iobuf *new_iobuf(int bufsize)
{
    struct iobuf *buf = NEW(struct iobuf);
    buf->buf = malloc_(bufsize);
    buf->pos = 0;
    buf->size = bufsize;
    return buf;
}
static void iobuf_init(struct iobuf *buf, int bufsize)
{
    buf->buf = malloc_(bufsize);
    buf->pos = 0;
    buf->size = bufsize;
}

static struct io *FILE_open_ (const char *name, const char *mode, int bufsize)
{
    struct io *io = (struct io *) malloc_(sizeof(struct io));
    IODRV_SET(io, FILE_read_, FILE_write_, FILE_close_, FILE_sync_);
    iobuf_init(&io->buf, 0);
    io->data_ = (void*)fopen(name, mode);
    io->is_r = (mode[0] == 'r')? 1: 0;
    io->is_w = (mode[0] == 'w'  || mode[0] == 'a')? 1: 0;
    return io;
}

static struct iodrv FILE_iodrv = {
    FILE_BUF_SIZE, FILE_read_, FILE_write_, FILE_close_, FILE_sync_, FILE_open_
};
enum iotype {
    FILE_IO
};

static void iodrv_add(const char *name, struct iodrv *drv)
{
    (void)name;
    Array_add(iodrv, g_iodrv, drv);
}

struct knh_OutputStream_t {
    knh_hObject_t  h;
    struct io     *io;
};
typedef struct knh_OutputStream_t knh_OutputStream_t;
static knh_OutputStream_t *new_OutputStream(knh_string_t *name, knh_string_t *mode)
{
    const char *md;
    knh_OutputStream_t *os = new_(OutputStream);
    if (!mode) {
        md = "w";
    } else {
        md = mode->txt;
    }
    os->io = new_io(name->txt, md, FILE_IO);
    return os;
}
static void OutputStream_delete(knh_OutputStream_t *os)
{
    os->io->sync_(os->io);
    delete_(os);
}

static void io_printf(struct io *io, int nargs, const char *fmt, ...)
{
    struct iobuf iobuf;
    char cbuf[256];
    int len;
    va_list ap;
    va_start(ap, fmt);
    len = vsnprintf(cbuf, 256, fmt, ap);
    iobuf.size = len;
    iobuf.pos  = 0;
    iobuf.buf  = (unsigned char *)cbuf;
    io->write_(io, &iobuf);
    va_end(ap);
}
static void io_puts(struct io *io, const char *cbuf, int len)
{
    struct iobuf buf;
    buf.size = len;
    buf.pos  = 0;
    buf.buf  = (unsigned char *)cbuf;
    io->write_(io, &buf);
#ifdef IO_DEBUG
    io->sync_(io);
#endif
}
static void io_gets(struct io *io, char *cbuf, int len)
{
    struct iobuf iobuf;
    iobuf.size = len;
    iobuf.pos  = 0;
    iobuf.buf  = (unsigned char *)cbuf;
    io->read_(io, &iobuf);
}

static void knh_object_write(struct io *io, knh_class_t cid, knh_Object_t *o)
{
    struct context *ctx = g_ctx;
    struct type_info *typeinfo = ctx->types + cid;
    typeinfo->write_(ctx, io, o);
}

static void OutputStream_print(knh_OutputStream_t *os, knh_class_t cid, knh_value_t v)
{
    struct io *io = os->io;
    char cbuf[128];
    if (cid == TYPE_Integer) {
        int size = snprintf(cbuf, 128, "%lld", v.ival);
        io_puts(io, cbuf, size);
        return;
    }
    else if(cid == TYPE_Float) {
        knh_float_t f = v.fval;
        int size = snprintf(cbuf, 128, "%f", f);
        io_puts(io, cbuf, size);
        return;
    }
    else if(cid == TYPE_String) {
        knh_string_t *s = cast(knh_string_t*, v.o);
        io_puts(io, s->txt, s->len);
        return;
    }
    knh_object_write(os->io, cid, v.o);
}
static void OutputStream_println(knh_OutputStream_t *os, knh_class_t cid, knh_value_t v)
{
    OutputStream_print(os, cid, v);
    io_puts(os->io, "\n", 1);
}

static void stream_init(void)
{
    g_iodrv = Array_new(iodrv);
    iodrv_add("file", &FILE_iodrv);
}

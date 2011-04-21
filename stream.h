#ifndef STREAM_H
#define STREAM_H

struct io;
static struct io *new_io(const char *name, const char *mode, int iotype);
static void io_puts(struct io *io, const char *cbuf, int len);
static void io_gets(struct io *io, char *cbuf, int len);
static void io_printf(struct io *io, int nargs, const char *fmt, ...);
static void io_delete(struct io *io);
static void stream_init(void);

#endif /* end of include guard: STREAM_H */

#include "stream.h"
#include "konoha.c"
#include "stream.c"

#ifndef KONOHA_MAIN
int main(int argc, char **argv)
{
    CTX ctx = new_context();
    g_ctx = cast(struct context*, ctx);
    g_iodrv = Array_new(iodrv);
    iodrv_add("file", &FILE_iodrv);
    //unsigned char cbuf[128];
    //struct iobuf buf = {128, 0, cbuf};
    //{
    //    struct io *io1 = new_io("stream_test.txt", "r", 0);
    //    fprintf(stderr, "============\n");
    //    io1->read_(io1, &buf);
    //    fprintf(stderr, "%s\n", buf.buf);
    //    fprintf(stderr, "============\n");
    //    io1->read_(io1, &buf);
    //    fprintf(stderr, "%s\n", buf.buf);
    //}
    //{
    //    struct io *io2 = new_io("/dev/stdout", "w", 0);
    //    fprintf(stderr, "============\n");
    //    io2->write_(io2, &buf);
    //    io2->write_(io2, &buf);
    //    io2->sync_(io2);
    //}
    {
        knh_string_t *n = new_string("/dev/stdout");
        knh_string_t *m = new_string("w");
        knh_value_t vi, vf, vo;
        knh_OutputStream_t *os = new_OutputStream(n, m);
        vi.ival = 12345;
        vf.fval = 12345.6789;
        vo.o = cast(knh_Object_t*, n);
        OutputStream_println(os, TYPE_Integer, vi);
        OutputStream_println(os, TYPE_Float, vf);
        OutputStream_println(os, TYPE_String, vo);
        OutputStream_delete(os);
    }
    return 0;
}
#endif

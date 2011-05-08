CC := gcc
build = ./build
objs = \
	build/konoha-lexer.o \
	build/konoha-paser.o \
	build/konoha.o

FLEX=flex
BISON=bison
KONOHA=$(build)/minikonoha
CFLAGS?=-I$(build) -I. -O0 -g3 -Wall -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -fno-common
#CFLAGS=-I$(build) -I. -O3 -g -Wall -Wwrite-strings -Wstrict-prototypes -Wmissing-prototypes -fno-common -D__N__=36
LDFLAGS=
INCLUDES=
KNH_HEADERS= \
	konoha.h \
	array.h  \
	tuple.h  \
	stream.h  \

bin = $(build)/vmtest $(KONOHA)

.PHONY: all
all : $(bin)

$(KONOHA) : $(objs) $(KNH_HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(KONOHA) $(objs)


build/konoha-lexer.o : ./konoha-lexer.l build/konoha-paser.o $(KNH_HEADERS)
	$(FLEX) -P konoha_ -o build/lexer.gen.c $<
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o $@ build/lexer.gen.c

build/konoha-paser.o : ./konoha-paser.y $(KNH_HEADERS)
	$(BISON) -p konoha_ --defines=build/y.gen.h --output=build/y.gen.c $<
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o $@ build/y.gen.c

build/konoha.o : ./konoha.c $(KNH_HEADERS) ./ctx.c ./stream.c vm.c kstring.c ./asm.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o $@ $<

build/vmtest: test/vm_test.c vm.c $(KNH_HEADERS) vmop.h vmbuilder.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -o $@ $<

./vm.c: vm.h vmbuilder.c

.PHONY: clean
clean:
	-rm -f $(build)/*.o $(build)/y.gen.{c,h} $(build)/lexer.gen.c $(bin)

.PHONY: test
test:
	$(build)/vmtest
	$(KONOHA) test/add.k
	$(KONOHA) test/print.k

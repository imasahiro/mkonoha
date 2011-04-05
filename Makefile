
build = ./build
objs = \
	build/konoha-lexer.o \
	build/konoha-paser.o \
	build/konoha.o

FLEX=flex
BISON=bison
KONOHA=minikonoha
CFLAGS=-I./build/ -I. -O0 -g3 -Wall
LDFLAGS=
KNH_HEADERS= \
	konoha.h \
	array.h  \
	tuple.h  \


.PHONY: all
all : $(KONOHA)

$(KONOHA) : $(objs) $(KNH_HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(KONOHA) $(objs)


build/konoha-lexer.o : ./konoha-lexer.l build/konoha-paser.o $(KNH_HEADERS)
	$(FLEX) -P konoha_ -o build/lexer.gen.c $<
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o $@ build/lexer.gen.c

build/konoha-paser.o : ./konoha-paser.y $(KNH_HEADERS)
	$(BISON) -p konoha_ --defines=build/y.gen.h --output=build/y.gen.c $<
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o $@ build/y.gen.c

build/konoha.o : ./konoha.c $(KNH_HEADERS) ./ctx.c
	$(CC) $(CFLAGS) $(INCLUDES) $(LDFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	-rm -f $(build)/*.o $(build)/y.gen.{c,h} $(build)/lexer.gen.c


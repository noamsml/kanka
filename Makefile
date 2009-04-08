OBJECT_FILES=mstring.o
TESTS=mstrtest
CFLAGS=--debug


all: tests

tests: $(TESTS)

mstrtest: mstring.o mstrtest.c
	gcc mstring.o mstrtest.c $(CFLAGS) -o mstrtest

mstring.o: mstring.c mstring.h
	gcc -c mstring.c -o mstring.o $(CFLAGS)

clean:
	-rm -f $(OBJECT_FILES) $(TESTS)
	


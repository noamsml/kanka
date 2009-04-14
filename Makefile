OBJECT_FILES=mstring.o blame.o
TESTS=mstrtest blametest
CFLAGS=--debug
BLAMEFLAG=-DBLAME

all: tests

tests: $(TESTS)

mstrtest: blame.o mstring.o mstrtest.c
	gcc mstring.o blame.o mstrtest.c $(CFLAGS) -o mstrtest

blametest: blame.o blametest.c
	gcc blame.o blametest.c $(CFLAGS) -o blametest

mstring.o: mstring.c mstring.h blame.h
	gcc -c mstring.c -o mstring.o $(CFLAGS)
	
blame.o: blame.c blame.h
	gcc -c blame.c -o blame.o $(CFLAGS)	$(BLAMEFLAG)

clean:
	-rm -f $(OBJECT_FILES) $(TESTS)
	


COMPILER = gcc
CSTD = --std=c18

shell : main.o shell.o
	$(COMPILER) $(CSTD) main.o shell.o -o shell

main.o : main.c shell.h
	$(COMPILER) $(CSTD) -c main.c

shell.o : shell.c shell.h
	$(COMPILER) $(CSTD) -c shell.c

clean :
	rm -f shell main.o shell.o
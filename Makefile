dlist: main.o dlist.o
	gcc -Wall -o dlist main.o dlist.o

main.o: main.c dlist.h
	gcc -Wall -c main.c

dlist.o: dlist.c dlist.h
	gcc -Wall -c dlist.c

clean:
	rm -rf *.o dlist

run: hash_table.o pre_assembler.o main.o
	gcc -g -ansi -pedantic -Wall main.o hash_table.o pre_assembler.o -o run

main.o: main.c
	gcc -ansi -pedantic -Wall -c main.c -o main.o

hash_table.o: hash_table.c hash_table.h constants.h
	gcc -ansi -pedantic -Wall -c hash_table.c -o hash_table.o

pre_assembler.o: pre_assembler.c
	gcc -ansi -pedantic -Wall -c pre_assembler.c -o pre_assembler.o 

clean:
	rm *.o

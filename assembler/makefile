pre_assembler: hash_table.o pre_assembler.o
	gcc -ansi -pedantic -Wall -o pre_assembler hash_table.o pre_assembler.o

hash_table.o: hash_table.c hash_table.h constants.h
	gcc -ansi -pedantic -Wall -c hash_table.c -o hash_table.o

pre_assembler.o: pre_assembler.c
	gcc -ansi -pedantic -Wall -c pre_assembler.c -o pre_assembler.o

clean:
	rm *.o
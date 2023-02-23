run_tests: hash_table.o tests.o
	gcc -g -ansi -pedantic -Wall  hash_table.o tests.o -o run

hash_table.o: ../hash_table.c ../hash_table.h ../constants.h
	gcc -ansi -pedantic -Wall -c ../hash_table.c -o hash_table.o

tests.o: tests.c
	gcc -ansi -pedantic -Wall -c tests.c -o tests.o

clean:
	rm *.o
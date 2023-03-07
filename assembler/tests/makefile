run_tests: hash_table.o encoder.o tests.o
	gcc -g -ansi -pedantic -Wall hash_table.o encoder.o tests.o -o run_tests

encoder.o: ../encoder.c ../encoder.h
	gcc -ansi -pedantic -Wall -c ../encoder.c -o encoder.o

hash_table.o: ../hash_table.c ../hash_table.h ../constants.h
	gcc -ansi -pedantic -Wall -c ../hash_table.c -o hash_table.o

tests.o: tests.c
	gcc -ansi -pedantic -Wall -c tests.c -o tests.o

clean:
	rm *.o
run_tests: tests.o encoder.o validate_file.o input_validation.o pre_assembler.o hash_table.o functions.o
	gcc -g -ansi -pedantic -Wall tests.o encoder.o validate_file.o input_validation.o pre_assembler.o hash_table.o functions.o -o run_tests

tests.o: tests.c ../encoder.h ../pre_assembler.h
	gcc -ansi -pedantic -Wall -c tests.c

encoder.o: ../encoder.c ../encoder.h ../validate_file.h
	gcc -ansi -pedantic -Wall -c ../encoder.c

validate_file.o: ../validate_file.c ../validate_file.h ../input_validation.h
	gcc -ansi -pedantic -Wall -c ../validate_file.c

input_validation.o: ../input_validation.c ../input_validation.h ../functions.h ../constants.h
	gcc -ansi -pedantic -Wall -c ../input_validation.c

pre_assembler.o: ../pre_assembler.c ../pre_assembler.h ../hash_table.h
	gcc -ansi -pedantic -Wall -c ../pre_assembler.c

functions.o: ../functions.c ../functions.h
	gcc -ansi -pedantic -Wall -c ../functions.c

hash_table.o: ../hash_table.c ../hash_table.h ../constants.h
	gcc -ansi -pedantic -Wall -c ../hash_table.c

clean:
	rm *.o


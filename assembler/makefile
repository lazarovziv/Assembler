run: main.o encoder.o validate_file.o input_validation.o pre_assembler.o hash_table.o functions.o errors.o
	gcc -g -ansi -pedantic -Wall main.o encoder.o validate_file.o input_validation.o pre_assembler.o hash_table.o functions.o errors.o -o run

main.o: main.c encoder.h pre_assembler.h
	gcc -ansi -pedantic -Wall -c main.c

encoder.o: encoder.c encoder.h validate_file.h
	gcc -ansi -pedantic -Wall -c encoder.c

validate_file.o: validate_file.c validate_file.h input_validation.h
	gcc -ansi -pedantic -Wall -c validate_file.c

input_validation.o: input_validation.c input_validation.h functions.h constants.h
	gcc -ansi -pedantic -Wall -c input_validation.c

pre_assembler.o: pre_assembler.c pre_assembler.h hash_table.h
	gcc -ansi -pedantic -Wall -c pre_assembler.c

functions.o: functions.c functions.h errors.h
	gcc -ansi -pedantic -Wall -c functions.c -o functions.o

hash_table.o: hash_table.c hash_table.h constants.h
	gcc -ansi -pedantic -Wall -c hash_table.c -o hash_table.o

errors.o: errors.c errors.h
	gcc -ansi -pedantic -Wall -c errors.c

clean:
	rm *.o *.ext *.ent *.ob


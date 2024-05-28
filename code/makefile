compile program: program.o
	@gcc -o program program.o -lm
program.o: program.c
	@gcc -c -Wall program.c
run: program
	@./program $(ARGS)
clean:
	@rm -f *.o
	@rm -f program
	@rm -f *.wav

compile program: program.o
	@gcc -o program program.o -lm
program.o: program.c
	@gcc -c -Wall program.c -lm
run: program
	@./program $(ARGS)
clean:
	@rm -f *.o
	@rm -f program

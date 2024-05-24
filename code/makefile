compile main: program.o
	@gcc -o program program.o -lm
main.o:
	@gcc program.c -c -Wall -lm
run: main
	@./program $(ARGS)
clean:
	@rm -f *.o
	@rm -f program

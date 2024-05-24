compile main: program.o
	@gcc -o program program.o
main.o:
	@gcc program.c -c -Wall
run: main
	@./program $(ARGS)
clean:
	@rm -f *.o
	@rm -f program
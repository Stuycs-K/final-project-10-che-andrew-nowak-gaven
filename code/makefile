compile program: program.o
	@gcc -o program program.o -lm
program.o: program.c
	@gcc -c -Wall program.c 
encode: program
	@./program encode $(ARGS)
decode: program
	@./program decode $(ARGS)
clean:
	@rm -f *.o
	@rm -f program
	@rm -f *.wav

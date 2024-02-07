all: tp.o arvore.o
	@gcc tp.o arvore.o -o exe
	@rm *.o
tp.o: tp.c
		@gcc tp.c -c
arvore.o: arvore.c
	@gcc arvore.c -c
run:
	@./exe
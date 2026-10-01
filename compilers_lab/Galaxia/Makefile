.SUFFIXES:

all: scanner

scanner: scanner.c lex.yy.c
		gcc scanner.c lex.yy.c -o scanner

lex.yy.c: scanner.l
		flex scanner.l

test: scanner
		@echo "\nPruebas de comentarios"
		./scanner < comentarios.txt
		@echo "\nPruebas de palabras reservadas"
		./scanner < keywords.txt
		@echo "\nPruebas de operadores"
		./scanner < operadores.txt
		@echo "\nPruebas de literales"
		./scanner < literals.txt
		@echo "\nPruebas de errores"
		./scanner < errores.txt
		@echo "\nPrueba completa"
		./scanner < prueba.txt 

clean:
		rm -f scanner lex.yy.c
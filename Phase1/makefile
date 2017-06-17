CC = gcc
FLEX = flex

all:
	$(FLEX) al.l
	$(CC) -o analyzer lex.yy.c

clean:
	rm -rf analyzer 
	rm -rf lex.yy.c
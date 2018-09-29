CC = gcc
OBJS = main.o Game.o Solver.o Parser.o linkedList.c
EXEC = sudoku
COMP_FLAG = -ansi -Wall -Wextra \
-Werror -pedantic-errors
LDFLAGS=-lm
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c Game.h Solver.h Parser.h SPBufferset.h
	$(CC) $(COMP_FLAG) -c $*.c
Solver.o: Solver.c Game.h Solver.h Parser.h
	$(CC) $(COMP_FLAG)  $(GUROBI_COMP) -c $*.c
Game.o: Game.c Game.h Solver.h Parser.h linkedList.h
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c Game.h Solver.h Parser.h
	$(CC) $(COMP_FLAG) -c $*.c
linkedList.o: linkedList.c Game.h Solver.h Parser.h
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)

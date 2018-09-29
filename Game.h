#ifndef GAME_H_
#define GAME_H_
#endif /* GAME_H_ */
#include<stdio.h>

 void addToArray(int** temp);/* adds the filled values calculated by the autofill function to the board */
 void solve(FILE *f);/* to enter solve mode */
 void edit(FILE *f);/* to enter edit mode */
 void markErrors(int i);/* sets the switch to mark boards with a '*' on (1) or off (0) byt the user */
 void printBoard();/* prints out the current board*/
 void set(char *str2,char *str3,char *str4);/* sets a value in a given cell and prints out the board */
 void validate();/* prints out if the board is valid and solvable or not */
 void generate(char *str1,char *str2);/* prints out a generated board according to the rules given */
 void undo();/* undoes the last done step */
 void redo();/* redoes the last undone step */
 void save(FILE *f,char *str);/* saves the current board in the file-name given by the user */
 void hint(char *str1,char *str2);/* prints out a hint for a given cell */
 void numOfSolutions();/* prints out the number of solutions */
 void autofill();/*autofills the board and prints it */
 void reset(bool isExit);/* Resets the board */
 void exit1();/* Exits from the game freeing all allocated memory */
 void freeMem(int **a);/* Frees the memory of the 2d array given */

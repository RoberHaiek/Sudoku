/*
 * Solver.h
 *
 *  Created on: May 21, 2018
 *      Author: rober
 */

#ifndef SOLVER_H_
#define SOLVER_H_
#include "gurobi_c.h"
#include<stdbool.h>

#endif /* SOLVER_H_ */

void freeObjVtype(double* obj,char* vtype);	/* Frees the 2 arrays from ILP */
void freeGenerate(int **a,int* b,int* c); /* Frees the arrays in GenerateX */
void arraysZeroing(int** marked,bool toZeroArray,bool toZeroMarked); /* meapeset the arrays in GenerateX, with flags pointing to which arrays to zero */
void push(int row,int coloumn);	/* push row,coloumn into the stack */
void pop();	/* pops last thing in stack */
bool isEmpty();	/* Checks if the stack is empty */
bool initialize(int x,int y);	/* Initializes a new board (GenerateXY) */
void removeY(int y);	/* Removes all but Y cells */
bool generateX(int x);	/* Generates X random cells */
void printB();	/* This function prints the solved board */
int chooseBlock(GRBmodel *model,int N,GRBenv *env);	/* Chooses values for each BLOCK */
int chooseijv(GRBmodel *model,int N,GRBenv *env);	/* Sets 1 for every value that's ALREADY in the model as sent from previous functions */
int chooseiforjv(GRBmodel *model,int N,GRBenv *env);	/* Chooses for every column and value, 1 row */
int choosejforiv(GRBmodel *model,int N,GRBenv *env);	/* Chooses for every row and value, 1 column */
int choosevforij(GRBmodel *model,int N,GRBenv *env);	/* Chooses for every row and column, 1 value */
int findSolution(GRBmodel *model,int N3,GRBenv *env,int optimstatus,double objval,double* sol);	/* Finds a solution to the module */
int ILPFunction(double* sol);	/* Integer Linear Programming function using Gurobi */
bool isValid();	/* Checks if the board is valid */
int getHint(int x,int y);	/* Gives the user a hint to the cell x,y */
bool setSolver(int i,int j,int v);	/* Checks if its legal to set v in the cell i,j */
int solverNumOfSolutions();	/* Find the number of solutions in the current board */
bool loop1(int i,int j); /* Checks which value is legal to set in i,j sets it and pushes to the stack */

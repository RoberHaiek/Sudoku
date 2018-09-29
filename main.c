/*
 * main.c
 *
 *  Created on: Sep 4, 2018
 *      Author: Weaam
 */
#include<stdio.h>
#include <stdlib.h>
#include"Parser.h"
#include"Game.h"
#include<string.h>
#include"SPBufferset.h"
#include<stdbool.h>
#include"linkedList.h"
#define N 1024
#define solveMode 0
#define editMode 1
#define initMode 2

extern struct Node* listPointer;
extern int mode;
extern bool markError;

int  main(){
	listPointer=NULL;
	mode=2;
	markError=1;
	SP_BUFF_SET();
	printf("Sudoku\n------\n");
	interpretCommand();
	return 1;
}

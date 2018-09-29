/*
 * Parser.h
 *
 *  Created on: Sep 3, 2018
 *      Author: Weaam
 */

#ifndef PARSER_H_
#define PARSER_H_
#endif /* PARSER_H_ */
#include<stdlib.h>
#include<stdbool.h>

void interpretCommand();	/*main func in parser. take command from user,read and interpret it */
bool checkIfSolveCommand(char array[]);	/* Checks if the command is solve */
bool checkIfEditCommand(char array[]);/* Checks if the command is edit */
bool checkIfMarkErrorsCommand(char array[]);/* Checks if the command is mark_errors */
bool checkIfPrintBoardCommand(char array[]);/* Checks if the command is print_board */
bool checkIfSetCommand(char array[]);/* Checks if the command is set */
bool checkIfValidateCommand(char array[]);/* Checks if the command is validate */
bool checkIfGenerateCommand(char array[]);/* Checks if the command is generate */
bool checkIfUndoCommand(char array[]);/* Checks if the command is undo */
bool checkIfRedoCommand(char array[]);/* Checks if the command is redo */
bool checkIfSaveCommand(char array[]);/* Checks if the command is save */
bool checkIfHintCommand(char array[]);/* Checks if the command is hint */
bool checkIfNumOfSolutionsCommand(char array[]);/* Checks if the command is num_solutions */
bool checkIfAutofillCommand(char array[]);/* Checks if the command is autofill */
bool checkIfResetCommand(char array[]);/* Checks if the command is reset */
bool checkIfExitCommand(char array[]);/* Checks if the command is exit */
bool checkifAnotherInvalidCommand(char input[]);/* Checks if the command is none of the above */

#include <ctype.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"Parser.h"
#include"Game.h"
#include"Solver.h"
#include"linkedList.h"
#define N 256
#define solveMode 0
#define editMode 1
#define initMode 2

extern int mode;
extern int m;
extern int n;


void interpretCommand(){
	char input[N];
	char inputForSolve[N];
	char inputForEdit[N];
	char inputForMarkErrors[N];
	char inputForPrintBoard[N];
	char inputForSet[N];
	char inputForValidate[N];
	char inputForGenerate[N];
	char inputForUndo[N];
	char inputForSave[N];
	char inputForHint[N];
	char inputForNumOfSolutions[N];
	char inputForAutofill[N];
	char inputForReset[N];
	char inputForExit[N];
	char inputforAnotherInvalidCommand[N];
	printf("Enter your command:\n");
	fgets(input,N,stdin);
	strcpy(inputForSolve,input);
	strcpy(inputForEdit,input);
	strcpy(inputForMarkErrors,input);
	strcpy(inputForPrintBoard,input);
	strcpy(inputForSet,input);
	strcpy(inputForValidate,input);
	strcpy(inputForGenerate,input);
	strcpy(inputForUndo,input);
	strcpy(inputForSave,input);
	strcpy(inputForHint,input);
	strcpy(inputForNumOfSolutions,input);
	strcpy(inputForAutofill,input);
	strcpy(inputForReset,input);
	strcpy(inputForExit,input);
	strcpy(inputforAnotherInvalidCommand,input);
	if(!checkIfSolveCommand(inputForSolve)){							interpretCommand();		}
	if(!checkIfEditCommand(inputForEdit)){								interpretCommand();		}
	if(!checkIfMarkErrorsCommand(inputForMarkErrors)){					interpretCommand();		}
	if(checkIfPrintBoardCommand(inputForPrintBoard)){
																		printB();
																		interpretCommand();		}
	if(!checkIfSetCommand(inputForSet)){								interpretCommand();		}
	if(checkIfValidateCommand(inputForValidate)){						validate();				}
	if(!checkIfGenerateCommand(inputForGenerate)){						interpretCommand();		}
	if(checkIfUndoCommand(inputForUndo)){								undo();					}
	if(checkIfRedoCommand(inputForUndo)){								redo();					}
	if(!checkIfSaveCommand(inputForSave)){								interpretCommand();		}
	if(!checkIfHintCommand(inputForHint)){								interpretCommand();		}
	if(checkIfNumOfSolutionsCommand(inputForNumOfSolutions)){			numOfSolutions();		}
	if(checkIfAutofillCommand(inputForAutofill)){						autofill();				}
	if(checkIfResetCommand(inputForAutofill)){							reset(0);				}
	if(checkIfExitCommand(inputForExit)){								exit1();				}
	if(!checkifAnotherInvalidCommand(inputforAnotherInvalidCommand)){	interpretCommand();		}
}

bool checkIfSolveCommand(char input[]){
	char *str1;
	char *str2;
	FILE *fs;
	char delimit[]=" \t\r\n\v\f";
	str1=strtok(input,delimit);
	if(strcmp(str1,"solve")==0){
		str2=strtok(NULL,delimit);
		if(str2=='\0'){
			printf("Error:invalid command\n");
			return 0;
		}
		fs=fopen(str2,"r");
		if(fs==NULL){
			printf("Error: File doesn't exist or cannot be opened\n");
			return 0;
		}
		solve(fs);
	}
	return 1;
}

bool checkIfEditCommand(char input[]){
	char *str1;
	char *str2;
	FILE *fe;
	char delimit[]=" \t\r\n\v\f";
	fe=NULL;

	str1=strtok(input,delimit);
	if(strcmp(str1,"edit")==0){
		str2=strtok(NULL,delimit);
		if(str2=='\0'){
			edit(fe);/*edit with NO X file*/
		}
		fe=fopen(str2,"r");
			if(fe==NULL){
				printf("Error: File cannot be opened\n");
				return 0;
			}
			edit(fe);/*edit with x file*/
	}
	return 1;
}

bool checkIfMarkErrorsCommand(char input[]){
	char *str1;
	char *str2;
	int i;
	char delimit[]=" \t\r\n\v\f";

	str1=strtok(input,delimit);
	if(strcmp(str1,"mark_errors")==0){
		if(mode!=solveMode){/*mode=0 means mode=solve*/
			printf("Error: invalid command\n");
			return 0;
		}
		str2=strtok(NULL,delimit);
		if(str2=='\0'){
			printf("Error:invalid command\n");
			return 0;
		}
		if(strcmp(str2,"0")!=0 && strcmp(str2,"1")!=0){
			printf("Error: the value should be 0 or 1\n");
			return 0;
		}
		i=(int)strtol(str2, (char **)NULL, 10);
		markErrors(i);
	}
	return 1;
}

bool checkIfPrintBoardCommand(char input[]){
	char *str1;
	char delimit[]=" \t\r\n\v\f";
	str1=strtok(input,delimit);
	if(strcmp(str1,"print_board")==0){
		if(mode!=solveMode && mode!=editMode){
			printf("Error: invalid command\n");
			interpretCommand();
		}
		return 1;
	}
	return 0;
}

bool checkIfSetCommand(char input[]){
			int isDigit,j;
			char *str1; /*first broken token by strtok*/
			char *str2;/*second*/
			char *str3;
			char *str4;
			char delimit[]=" \t\r\n\v\f";
			j=0;
			str1=strtok(input,delimit);
			if(strcmp(str1,"set")==0){
				if(mode!=solveMode && mode!=editMode){
					printf("Error: invalid command\n");
					return 0;
				}
				str2=strtok(NULL,delimit);
				str3=strtok(NULL,delimit);
				str4=strtok(NULL,delimit);
				if(str2=='\0' || str3=='\0' || str4=='\0'){
					printf("Error: invalid command\n");
					return 0;
				}

			/*	 the next 3 while loops to check if x y z are integers
 * 				to be taken care of in game modeulo because i need to know the size of the board*/

				while(j<(int) strlen(str2)){
				  isDigit = isdigit(str2[j]);
				  if (isDigit == 0){
					  printf("Error: value not in range 0-%d\n",m*n);
					  return 0;
				  }
				  else{
					  isDigit=(int)strtol(str2, (char **)NULL, 10);
					  if(isDigit<1 || isDigit>m*n){
						  printf("Error: value not in range 0-%d\n",m*n);
						  return 0;
								  }
							  }
				  j++;
				}
				j=0;
				while(j<(int) strlen(str3)){
					  isDigit = isdigit(str3[j]);
					  if (isDigit == 0){
						  printf("Error: value not in range 0-%d\n",m*n);
						  return 0;
					  }
					  else{
						  isDigit=(int)strtol(str3, (char **)NULL, 10);
						  if(isDigit<1 || isDigit>m*n){
							  printf("Error: value not in range 0-%d\n",m*n);
							  return 0;
						  }
					  }
					  j++;
					}
				j=0;
				while(j<(int) strlen(str4)){
					  isDigit = isdigit(str4[j]);
					  if (isDigit == 0){
						  printf("Error: value not in range 0-%d\n",m*n);
						  return 0;
					  }
					  else{
				       	  isDigit=(int)strtol(str3, (char **)NULL, 10);
					      if(isDigit<1 || isDigit>m*n){
						   printf("Error: value not in range 0-%d\n",m*n);
						   return 0;
						  }
					  }
					  j++;
					}
				set(str2,str3,str4);
			}
			return 1;
}

bool checkIfValidateCommand(char input[]){
	char *str1;
	char delimit[]=" \t\r\n\v\f";
	str1=strtok(input,delimit);
	if(strcmp(str1,"validate")==0){
		if(mode!=solveMode && mode!=editMode){
			printf("Error: invalid command\n");
			interpretCommand();
		}
		return 1;
	}
	return 0;
}

bool checkIfGenerateCommand(char input[]){
	char *str1; /*first broken token by strtok*/
	char *str2;/*second*/
	char *str3;
	char delimit[]=" \t\r\n\v\f";
	str1=strtok(input,delimit);
	if(strcmp(str1,"generate")==0){
		if(mode!=editMode){
			printf("Error: invalid command\n");
			return 0;
		}
		str2=strtok(NULL,delimit);
		str3=strtok(NULL,delimit);
		if(str2=='\0' || str3=='\0'){
			printf("Error: invalid command\n");
			return 0;
		}
		generate(str2,str3);
	}
	return 1;
}

bool checkIfUndoCommand(char input[]){
	char *str1;
	char delimit[]=" \t\r\n\v\f";
	str1=strtok(input,delimit);
	if(strcmp(str1,"undo")==0){
		if(mode!=solveMode && mode!=editMode){
			printf("Error: invalid command\n");
			interpretCommand();
		}
		return 1;
	}
	return 0;
}

bool checkIfRedoCommand(char input[]){
	char *str1;
	char delimit[]=" \t\r\n\v\f";
	str1=strtok(input,delimit);
	if(strcmp(str1,"redo")==0){
		if(mode!=solveMode && mode!=editMode){
			printf("Error: invalid command\n");
			interpretCommand();
		}
		return 1;
	}
	return 0;
}

bool checkIfSaveCommand(char input[]){
	char *str1;
	char *str2;
	FILE *fs;
	char delimit[]=" \t\r\n\v\f";
	str1=strtok(input,delimit);
	if(strcmp(str1,"save")==0){
		if(mode!=solveMode && mode!=editMode){
			printf("Error: invalid command\n");
			return 0;
		}
		str2=strtok(NULL,delimit);
		if(str2=='\0'){
			printf("Error:invalid command\n");
			return 0;
		}
		fs=fopen(str2,"w+");
		if(fs==NULL){
			printf("Error: File cannot be created or modified\n");
			return 0;
		}
		save(fs,str2);
	}
	return 1;
}

bool checkIfHintCommand(char input[]){
	int isDigit,j;
	char *str1; /*first broken token by strtok*/
	char *str2;/*second*/
	char *str3;
	char delimit[]=" \t\r\n\v\f";
	str1=strtok(input,delimit);
	if(strcmp(str1,"hint")==0){
		if(mode!=solveMode){
			printf("Error: invalid command\n");
			return 0;
		}
		str2=strtok(NULL,delimit);
		str3=strtok(NULL,delimit);
		if(str2=='\0' || str3=='\0'){
			printf("Error: invalid command\n");
			return 0;
		}
		j=0;
		while(j<(int) strlen(str2)){
		  isDigit = isdigit(str2[j]);
		  if (isDigit == 0){
			printf("Error: invalid command\n");
			  return 0;
			 }
			 j++;
			}
		j=0;
		while(j<(int) strlen(str3)){
			  isDigit = isdigit(str3[j]);
			  if (isDigit == 0){
			printf("Error: invalid command\n");
				  return 0;
			  }
			  j++;
			}

		hint(str2,str3);
	}
	return 1;
}

bool checkIfNumOfSolutionsCommand(char input[]){
	char *str1;
	char delimit[]=" \t\r\n\v\f";
	str1=strtok(input,delimit);
	if(strcmp(str1,"num_solutions")==0){
		if(mode!=solveMode && mode!=editMode){
			printf("Error: invalid command\n");
			interpretCommand();
		}
		return 1;
	}
	return 0;
}

bool checkIfAutofillCommand(char input[]){
	char *str1;
	char delimit[]=" \t\r\n\v\f";
	str1=strtok(input,delimit);
	if(strcmp(str1,"autofill")==0){
		if(mode!=solveMode){
			printf("Error: invalid command\n");
			interpretCommand();
		}
		return 1;
	}
	return 0;
}

bool checkIfResetCommand(char input[]){
	char *str1;
	char delimit[]=" \t\r\n\v\f";
	str1=strtok(input,delimit);
	if(strcmp(str1,"reset")==0){
		if(mode!=solveMode && mode!=editMode){
			printf("Error: invalid command\n");
			interpretCommand();
		}
		return 1;
	}
	return 0;
}

bool checkIfExitCommand(char input[]){
		char *str1;
		char delimit[]=" \t\r\n\v\f";
		str1=strtok(input,delimit);
		if(strcmp(str1,"exit")==0){
			return 1;
		}
		return 0;
}

bool checkifAnotherInvalidCommand(char input[]){
	char *str1;
	char delimit[]=" \t\r\n\v\f";
	str1=strtok(input,delimit);
	if((strcmp(str1,"exit")!=0) && (strcmp(str1,"set")!=0) && (strcmp(str1,"reset")!=0) && (strcmp(str1,"hint")!=0) && (strcmp(str1,"validate")!=0) && (strcmp(str1,"redo")!=0) && (strcmp(str1,"undo")!=0 ) && (strcmp(str1,"generate")!=0) && (strcmp(str1,"num_solutions")!=0) && (strcmp(str1,"autofill")!=0) && (strcmp(str1,"save")!=0) && (strcmp(str1,"solve")!=0) && (strcmp(str1,"edit")!=0) && (strcmp(str1,"mark_errors")!=0)){
		printf("Error: invalid command\n");
		return 0;
	}
	return 1;
}

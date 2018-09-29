#include"Solver.h"
#include"Game.h"
#include<stdbool.h>
#include"Parser.h"
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include"linkedList.h"
#define N 256
#define solveMode 0
#define editMode 1
#define initMode 2


void undoPrints(int a,int b,int c,int d);/* prints out the "undo" outputs */
void redoPrints(int a,int b,int c,int d);/* prints out the "redo" outputs */
bool filledBoard();/* Checks if the board is full*/
void emptyLinkedList();/* empties the linked list */
int getNumberOfEmptyCells();/* gets the total number of empty cells in the board */
void fileToArray(FILE *fta);/* converts a file to array for easier usage */
void checkBoardErroneous();/* checks if the board has any errors */
bool erroneous();
int **array,**fixed,**astrex;
struct Node *listPointer;
int counterForAutofill;
int mode;
bool markError;
int m,n;
int isItFirstNode=1;


void solve(FILE *fs){
	int k;
	mode=solveMode;
	emptyLinkedList();/*to delete all nodes in redo undo list*/
	fscanf(fs,"%d",&m);
	fscanf(fs,"%d",&n);
	array=(int **)calloc(m*n,sizeof(int*));
	fixed=(int **)calloc(m*n,sizeof(int*));
	astrex=(int **)calloc(m*n,sizeof(int*));
	for(k=0;k<m*n;k++){
		array[k]=(int *)calloc(m*n,sizeof(int));
		fixed[k]=(int *)calloc(m*n,sizeof(int));
		astrex[k]=(int *)calloc(m*n,sizeof(int));
	}
	fileToArray(fs);
	fclose(fs);
	interpretCommand();
}

void edit(FILE *fe){
	int k;
	mode=editMode;
	emptyLinkedList();
	if(fe==NULL){/*no x file given*/
		m=3;
		n=3;
		array=(int **)calloc(m*n,sizeof(int*));
			fixed=(int **)calloc(m*n,sizeof(int*));
			astrex=(int **)calloc(m*n,sizeof(int*));
			for(k=0;k<m*n;k++){
				array[k]=(int *)calloc(m*n,sizeof(int));
				fixed[k]=(int *)calloc(m*n,sizeof(int));
				astrex[k]=(int *)calloc(m*n,sizeof(int));
			}
	}
	else{/*with x file*/
		fscanf(fe,"%d",&m);
		fscanf(fe,"%d",&n);

		array=(int **)calloc(m*n,sizeof(int*));
		fixed=(int **)calloc(m*n,sizeof(int*));
		astrex=(int **)calloc(m*n,sizeof(int*));
		for(k=0;k<m*n;k++){
			array[k]=(int *)calloc(m*n,sizeof(int));
			fixed[k]=(int *)calloc(m*n,sizeof(int));
			astrex[k]=(int *)calloc(m*n,sizeof(int));
		}
		fileToArray(fe);
		fclose(fe);
	}
	interpretCommand();
}

void markErrors(int mark){
	markError=mark;
	printB();
	interpretCommand();
}

void printBoard(){
	void printB();/*to be implemented in solver, make extern arrays*/
	interpretCommand();
}

void set(char *X,char *Y,char *Z){
	char input[N];
	int x,y,z;
	x=(int)strtol(X, (char **)NULL, 10);
	y=(int)strtol(Y, (char **)NULL, 10);
	z=(int)strtol(Z, (char **)NULL, 10);
	if((z<0 || z>m*n)){
		printf("Error: value not in range 0-%d\n",m*n);
		interpretCommand();

	}
	if(mode==solveMode){
		if(fixed[y-1][x-1] && mode==solveMode){
			printf("Error: cell is fixed\n");
			interpretCommand();
			
		}
	}
	/*take care of the undoredo list*/
	if(listPointer!=NULL){
		while(listPointer->next!=NULL){ /*deleting the commands that were 'undo'*/
			 deleteNode(&listPointer, listPointer->next);
		 }
	}
	append(&listPointer, "set", x, y, z, array[y-1][x-1]);
	array[y-1][x-1]=z;
	checkBoardErroneous();
	printB();
	if(mode==solveMode){
		if(filledBoard()){
			if(erroneous()){
				printf("Puzzle solution erroneous\n");
				printf("Enter your command:\n");
				fgets(input,N,stdin);
				while(!strcmp(input,"undo")){
					fgets(input,N,stdin);
				}
				undo();
				interpretCommand();
			}
			else{
				if(isValid()){
					printf("Puzzle solved successfully\n");
					mode=initMode;
					interpretCommand();
				}
			}
		}
	}
	interpretCommand();
}

void validate(){

	if(erroneous()){
		printf("Error: board contains erroneous values\n");
		interpretCommand();
	}

	if(isValid()){/*isValid to be implemented in solver*/
		printf("Validation passed: board is solvable\n");
		interpretCommand();
	}
	else{
		printf("Validation failed: board is unsolvable\n");
		interpretCommand();
	}
}

void generate(char *str2,char *str3){
	int numOfEmptyCells, isDigit, X,Y;
	int j;
	j=0;
	numOfEmptyCells=getNumberOfEmptyCells();
	while(j<(int) strlen(str2)){
	  isDigit = isdigit(str2[j]);
	  if (isDigit == 0){
		  printf("Error: value not in range 0-%d\n",numOfEmptyCells);
		  interpretCommand();
		 }
		 j++;
		}
	X=(int)strtol(str2, (char **)NULL, 10);
	if(X> numOfEmptyCells){
		  printf("Error: value not in range 0-%d\n",numOfEmptyCells);
		  interpretCommand();
	}
	j=0;
	while(j<(int) strlen(str3)){
		  isDigit = isdigit(str3[j]);
		  if (isDigit == 0){
			  printf("Error: value not in range 0-%d\n",numOfEmptyCells);
			  interpretCommand();
		  }
		  j++;
		}

	Y=(int)strtol(str3, (char **)NULL, 10);
	if(Y > numOfEmptyCells){
		  printf("Error: value not in range 0-%d\n",numOfEmptyCells);
		  interpretCommand();
	}

	if(numOfEmptyCells<(m*n*m*n)){
		printf("Error: board is not empty\n");
		interpretCommand();
	}

	if(initialize(X,Y)){/*initialize to be implemented in solver*/
		printB();
		interpretCommand();
	}
	else{
		printf("Error: puzzle generator failed\n");
		interpretCommand();
	}
}

void undo(){
	int i,*temp,c=0;
	temp=(int *)calloc(4*n*m*n*m,sizeof(int));
	i=counterForAutofill;
	if(listPointer==NULL){
		printf("Error: no moves to undo\n");
		free(temp);
		interpretCommand();
	}
	if(listPointer->prev!=NULL){
		if(!strcmp(listPointer->command,"set")){
			array[listPointer->Y-1][listPointer->X-1]=listPointer->oldZ;
			printB();
			undoPrints(listPointer->X,listPointer->Y,listPointer->newZ,listPointer->oldZ);
			listPointer=listPointer->prev;
			checkBoardErroneous();
			free(temp);
			interpretCommand();
		}
		else if(!strcmp(listPointer->command,"autofill")){
					while( listPointer->prev!=NULL && !strcmp(listPointer->command,"autofill") && i==counterForAutofill){
						array[listPointer->Y-1][listPointer->X-1]=listPointer->oldZ;
						temp[c]=listPointer->X;
						temp[c+1]=listPointer->Y;
						temp[c+2]=listPointer->newZ;
						temp[c+3]=listPointer->oldZ;
						c+=4;
						listPointer=listPointer->prev;
					}
					printB();
					for(i=0;i<c;i+=4){
						undoPrints(temp[i],temp[i+1],temp[i+2],temp[i+3]);
					}
					free(temp);
					checkBoardErroneous();
					interpretCommand();
				}
		else if(!strcmp(listPointer->command,"generate")){
			while( listPointer->prev!=NULL && !strcmp(listPointer->command,"generate")){
				array[listPointer->Y-1][listPointer->X-1]=listPointer->oldZ;
						temp[c]=listPointer->X;
						temp[c+1]=listPointer->Y;
						temp[c+2]=listPointer->newZ;
						temp[c+3]=listPointer->oldZ;
						c+=4;
						listPointer=listPointer->prev;
					}
					printB();
					for(i=0;i<c;i+=4){
						undoPrints(temp[i],temp[i+1],temp[i+2],temp[i+3]);
					}
					free(temp);
			interpretCommand();
		}
	}
	else{
		printf("Error: no moves to undo\n");
		free(temp);
		interpretCommand();
	}
}

void undoPrints(int a,int b,int c,int d){
	if(c!=0 && d!=0){
		checkBoardErroneous();
		printf("Undo %d,%d: from %d to %d\n",a,b,c,d);
	}
	if(c==0 && d!=0){
		checkBoardErroneous();
		printf("Undo %d,%d: from %s to %d\n",a,b,"'_'",d);
	}
	if(c !=0 && d==0){
		checkBoardErroneous();
		printf("Undo %d,%d: from %d to %s\n",a,b,c,"'_'");
	}
	if(c==0 && d==0){
		checkBoardErroneous();
		printf("Undo %d,%d: from %s to %s\n",a,b,"''","''");
	}
}

void redo(){
	int i,*temp,c=0;
	temp=(int *)calloc(4*n*m*n*m,sizeof(int));
	i=counterForAutofill;
	if(listPointer==NULL){
		printf("Error: no moves to redo\n");
		free(temp);
		interpretCommand();
	}
		if(listPointer->next==NULL){
			printf("Error: no moves to redo\n");
			free(temp);
			interpretCommand();
		}
		else{
		if(!strcmp(listPointer->next->command,"set")){
			listPointer=listPointer->next;
			array[listPointer->Y-1][listPointer->X-1]=listPointer->newZ;
			printB();
			redoPrints(listPointer->X,listPointer->Y,listPointer->newZ,listPointer->oldZ);
			checkBoardErroneous();
			free(temp);
			interpretCommand();
		}
		else if(!strcmp(listPointer->next->command,"autofill") && i==counterForAutofill){
			while( listPointer->next!=NULL && !strcmp(listPointer->next->command,"autofill")){
				listPointer=listPointer->next;
				array[listPointer->Y-1][listPointer->X-1]=listPointer->newZ;
				temp[c]=listPointer->X;
				temp[c+1]=listPointer->Y;
				temp[c+2]=listPointer->newZ;
				temp[c+3]=listPointer->oldZ;
				c+=4;
			}
			printB();
			for(i=0;i<c;i+=4){
				redoPrints(temp[i],temp[i+1],temp[i+2],temp[i+3]);
			}
			free(temp);
			checkBoardErroneous();
			interpretCommand();
		}
		else if(!strcmp(listPointer->next->command,"generate")){
			while( listPointer->next!=NULL && !strcmp(listPointer->next->command,"generate")){
				listPointer=listPointer->next;
				array[listPointer->Y-1][listPointer->X-1]=listPointer->newZ;
				temp[c]=listPointer->X;
				temp[c+1]=listPointer->Y;
				temp[c+2]=listPointer->newZ;
				temp[c+3]=listPointer->oldZ;
				c+=4;
			}
			printB();
			for(i=0;i<c;i+=4){
				redoPrints(temp[i],temp[i+1],temp[i+2],temp[i+3]);
			}
			free(temp);
			checkBoardErroneous();
			interpretCommand();}
	}
}

void redoPrints(int a,int b,int c,int d){
	if(c!=0 && d!=0){
		checkBoardErroneous();
		printf("Redo %d,%d: from %d to %d\n",a,b,c,d);
	}
	if(c==0 && d!=0){
		checkBoardErroneous();
		printf("Redo %d,%d: from %s to %d\n",a,b,"'_'",d);
	}
	if(c !=0 && d==0){
		checkBoardErroneous();
		printf("Redo %d,%d: from %d to %s\n",a,b,c,"'_'");
	}
	if(c==0 && d==0){
		checkBoardErroneous();
		printf("Redo %d,%d: from %s to %s\n",a,b,"''","''");
	}
}

void save(FILE *fs,char *str){
	int i,j;
	int firstD,secondD;
	if(mode==editMode){
		for(i=0;i<m*n;i++){
			for(j=0;j<m*n;j++){
				if(astrex[i][j]){
					printf("Error: board contains erroneous values\n");
					interpretCommand();
				}
			}
		}
		if(!isValid()){
			printf("Error: board validation failed\n");
			interpretCommand();
		}
	}
	/*write to file*/
	fputc(m + '0',fs);
	fputc(32,fs);/*space*/
	fputc(n +'0',fs);
	fputc(10,fs);/*new line*/

	for(i=0;i<m*n;i++){
		for(j=0;j<m*n;j++){
			if(array[i][j]<10){
				fputc(array[i][j] + '0',fs);
			}
			else if(array[i][j]>=10 && array[i][j]<=99){
				firstD=array[i][j]/10;
				secondD=array[i][j]%10;
				fputc(firstD + '0',fs);
				fputc(secondD+ '0',fs);
			}
			if(mode==editMode){
				if(array[i][j]){
					fputc(46,fs);/*add '.'*/
				}
			}
			if(mode==solveMode){
				if(fixed[i][j]){
					fputc(46,fs);/*add '.'*/
				}
			}
			if(j<m*n-1){
				fputc(32,fs);
			}
		}
		if(i<m*n-1){
			fputc(10,fs);
		}
	}
	printf("Saved to: %s\n",str);
	fclose(fs);
	interpretCommand();


}

void hint(char *X,char *Y){
	int x,y;
	int i,j;
	int hint;
	x=(int)strtol(X, (char **)NULL, 10);
	y=(int)strtol(Y, (char **)NULL, 10);
	if((x<1 ||x>m*n) || (y<1 || y>m*n)){
		printf("Error: value not in range 1-%d\n",m*n);
		interpretCommand();
	}
	for(i=0;i<m*n;i++){
		for(j=0;j<m*n;j++){
			if(astrex[i][j]){
				printf("Error: board contains erroneous values\n");
				interpretCommand();
			}

		}
	}
	if(fixed[y-1][x-1]){
		printf("Error: cell is fixed\n");
			interpretCommand();
		}
	if(array[y-1][x-1]){
		printf("Error: cell already contains a value\n");
		interpretCommand();
		}
	hint=getHint(y-1,x-1);/*to be updated in ilpSolver*/
		if(hint==0){
		printf("Error: board is unsolvable\n");
		interpretCommand();
	}
	printf("Hint: set cell to %d\n",hint);
	interpretCommand();
}

void numOfSolutions(){
	int num;
	if(erroneous()){
		printf("Error: board contains erroneous values\n");
		interpretCommand();
	}
	num=solverNumOfSolutions(); /* to be implemented in solver*/
	printf("Number of solutions: %d\n",num);
	if(num==1){
		printf("This is a good board!\n");
	}
	else{
		printf("The puzzle has more than 1 solution, try to edit it further\n");
	}
	interpretCommand();
}

void autofill(){
	int k,i,j,sum=0,*legal,**temp;
	temp =(int **)calloc(n*m,sizeof(int*));
	for (i=0; i<m*n; i++){
		 temp[i] = (int *)calloc(n*m,sizeof(int));
	}
	legal=(int *)calloc(n*m*n*m*n*m,sizeof(int));
	if(erroneous()){
		free(legal);
		freeMem(temp);
		printf("Error: board contains erroneous values\n");
		interpretCommand();
	}
	for(i=0;i<n*m;i++){
		for(j=0;j<n*m;j++){
			if(array[i][j]==0){
				for(k=1;k<(n*m)+1;k++){	/** check all the legal values for the specific cell (k is the number of legal values) **/
					if(setSolver(i,j,k)){
						legal[sum]=k;
						sum++;
					}
				}
				if(sum==1){
					temp[i][j]=legal[0];
					printf("Cell <%d,%d> set to %d\n",j+1,i+1,legal[0]);
					append(&listPointer, "autofill", j+1, i+1,legal[0], 0);
				}
				for(sum=0;sum<n*m;sum++){
					legal[sum]=0;	}
				sum=0;
			}
		}
	}
	addToArray(temp);
	if(mode==solveMode){
		if(filledBoard()){
			if(erroneous()){
				printf("Puzzle solution erroneous\n");
				printf("Enter your command:\n");
				free(legal);
				freeMem(temp);
				interpretCommand();
			}
			else{
				if(isValid()){
					printB();
					printf("Puzzle solved successfully\n");
					mode=initMode;
					free(legal);
					freeMem(temp);
					interpretCommand();
				}
			}
		}
	}
	free(legal);
	freeMem(temp);
	printB();
	interpretCommand();
}

void addToArray(int** temp){
	int i,j;
	for(i=0;i<n*m;i++){
		for(j=0;j<n*m;j++){
			if(temp[i][j]){
				array[i][j]=temp[i][j];
			}
		}
	}
}

void reset(bool isExit){
	isItFirstNode=1;
	if(listPointer!=NULL){
		while(listPointer->next!=NULL){/*deleting the commands that were 'undone' */
			deleteNode(&listPointer, listPointer->next);
			
		 	 }
		 while(listPointer->prev!=NULL){
			 array[listPointer->Y-1][listPointer->X-1]=listPointer->oldZ;
			 listPointer=listPointer->prev;
			 deleteNode(&listPointer, listPointer->next);
		 }
	 	 deleteNode(&listPointer,listPointer);
	}
		if(!isExit){
		 printf("Board reset\n");
		checkBoardErroneous();
		 interpretCommand();
		}

}

void exit1(){
	reset(1);
	freeMem(array);
	freeMem(fixed);
	freeMem(astrex);
	printf("Exiting...\n");
	exit(1);
}

void fileToArray(FILE *fs){
	int i,j;
    char c;
    int d;
	fgetc(fs);/* this char is '\n'*/
	for(i=0;i<m*n+1;i++){/* m*n+1 to take care if the last char in the file is '.'*/
		for(j=0;j<m*n;j++){
			c=fgetc(fs);
			if(c=='.'){
				if(j>0){
				fixed[i][j-1]=1;
				}
				if(j==0){
					fixed[i-1][m*n-1]=1;
				}
				c=fgetc(fs);
			}
			if(c==' '){
				c=fgetc(fs);
			}
			if(c=='\n'){
				c=fgetc(fs);
			}
			if(isdigit(c)){
				d=c - '0';
				c=fgetc(fs);
				if(isdigit(c)){
					d=d*10 +(c-'0');
					array[i][j]=d;


				}
				if(!isdigit(c)){
				array[i][j]=d;
				}
				if(c=='.'){
					fixed[i][j]=1;
					c=fgetc(fs);
				}
			}
		}
	}
	checkBoardErroneous();
}

bool erroneous(){
	int i,j;
	for(i=0;i<m*n;i++){
		for(j=0;j<m*n;j++){
			if(astrex[i][j]){
				return 1;
			}
		}
	}
	return 0;
}

void freeMem(int **a) {
    int i;
    for (i = 0; i < m*n; ++i) {
        free(a[i]);
    }
    free(a);
}

void checkBoardErroneous(){
	int i,j,k,l,checkRow,checkColumn;
	for(i=0;i<m*n;i++){
		for(j=0;j<m*n;j++){
			astrex[i][j]=0;
		}
	}
	for(i=0;i<m*n;i++){
		for(j=0;j<m*n;j++){

			for(k=0;k<m*n;k++){
				if(array[i][k]==array[i][j] && array[i][j]!=0 && k!=j){
					astrex[i][k]=1;
					astrex[i][j]=1;
				}
			}
			for(k=0;k<m*n;k++){
				if(array[k][j]==array[i][j] &&array[i][j]!=0 && k!=i){
					astrex[k][j]=1;
					astrex[i][j]=1;
					}
			}
			/*check blocks*/
			checkRow=1;
			checkColumn=1;
        	while(checkRow*(m*n)/m<=m*n){
				if((i+1)>=1+(checkRow-1)*m && (i+1)<=checkRow*((m*n)/n)){
					while(checkColumn*(m*n)/n<=m*n){
						if((j+1)>=1+(checkColumn-1)*n && (j+1)<=checkColumn*((m*n)/m)){
							for(k=(checkRow-1)*m;k<checkRow*m;k++){
								for(l=(checkColumn-1)*n;l<checkColumn*n;l++){
							    	if(array[k][l]==array[i][j] && array[i][j]!=0 && ((k!=i) || (l!=j))){
										astrex[k][l]=1;
										astrex[i][j]=1;
									}
								}

							}
						}
						checkColumn++;
					}
				}
				checkRow++;
			}
		}
	}
	return;

}

int getNumberOfEmptyCells(){
	int numberOfEmptyCells;
	int i,j;
	numberOfEmptyCells=0;
	for(i=0;i<m*n;i++){
		for(j=0;j<m*n;j++){
			if(array[i][j]==0){
				 numberOfEmptyCells++;
			}
		}
	}
	return numberOfEmptyCells;
}

void emptyLinkedList(){
	if(!(listPointer==NULL)){
		 while(listPointer->next!=NULL){/*deleting the commands that were 'undo' */
			 deleteNode(&listPointer, listPointer->next);
		 }
		
		 while(listPointer->prev!=NULL){
			 listPointer=listPointer->prev;
		 	deleteNode(&listPointer, listPointer->next);
	 	}
 	 	deleteNode(&listPointer,listPointer);
	}
	isItFirstNode=1;
	return;
	
}

bool filledBoard(){
	int i,j;
	for(i=0;i<m*n;i++){
		for(j=0;j<m*n;j++){
			if(array[i][j]==0){
				return 0;
			}
		}
	}
	return 1;
}

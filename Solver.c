#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "Solver.h"
#include "linkedList.h"
#include "Game.h"
#include "gurobi_c.h"
#include "Parser.h"
#include <math.h>
#include <stdbool.h>
#define solveMode 0
#define editMode 1
extern int n;
extern int m;
extern int **array;
extern int **fixed;
extern int mode;
extern bool markError;
extern int** astrex;
extern struct Node* listPointer;
int C;


typedef struct StackNode{
	int row;
	int coloumn;
	int value;
	struct StackNode* next;
}StackNode;

StackNode* top;

/* Frees the 2 arrays from ILP */

void freeObjVtype(double* obj,char* vtype){
	free(obj);
	free(vtype);
}

/* Integer Linear Programming function using Gurobi */

int ILPFunction(double* sol)
{
  GRBenv   *env   = NULL;
  GRBmodel *model = NULL;
  int       N=n*m, error = 0,N3=(int)pow(N,3),optimstatus=0,i;
  double    objval=0.0,*obj=(double *)calloc(N3,sizeof(double));
  char      *vtype=(char *)calloc(N3,sizeof(char));
  error = GRBloadenv(&env, "mip1.log");
  if (error) {
	  printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
	  freeObjVtype(obj,vtype);
	  return 0;
  }
  error=GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
  if (error) {
	  printf("ERROR %d GRBsetintparam(): %s\n", error, GRBgeterrormsg(env));
	  freeObjVtype(obj,vtype);
	  return 0;
  }

  error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
  if (error) {
	  printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
	  freeObjVtype(obj,vtype);
	  return 0;
  }
  for(i=0;i<N3;i++){
	  vtype[i]=GRB_BINARY;
  }
  error = GRBaddvars(model, N3, 0, NULL, NULL, NULL, obj, NULL, NULL, vtype, NULL);
  if (error) {
	  printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
	  freeObjVtype(obj,vtype);
	  return 0;
  }
  error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MINIMIZE);
  if (error) {
	  printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
	  freeObjVtype(obj,vtype);
	  return 0;
  }
  error = GRBupdatemodel(model);
  if (error) {
	  printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
	  freeObjVtype(obj,vtype);
	  return 0;
  }
  if(!choosevforij(model,N,env)||!choosejforiv(model,N,env)||!chooseiforjv(model,N,env)||!chooseBlock(model,N,env)||!chooseijv(model,N,env)||!findSolution(model,N3,env,optimstatus,objval,sol)){
	  freeObjVtype(obj,vtype);
	  return 0;
  }

  GRBfreemodel(model);
  GRBfreeenv(env);
  freeObjVtype(obj,vtype);
  return 1;
}

/* Finds a solution to the module */

int findSolution(GRBmodel *model,int N3,GRBenv *env,int optimstatus,double objval,double* sol) {
	int error=0;
	error = GRBoptimize(model);
	  if (error) {
		  printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		  return 0;
	  }
	  error = GRBwrite(model, "mip1.lp");
	  if (error) {
		  printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
		  return 0;
	  }
	  error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	  if (error) {
		  printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		  return 0;
	  }
	  error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	  if (error) {
		  printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));
		  return 0;
	  }
	  error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, N3, sol);
	  if (error) {
		  printf("ERROR %d GRBgetdblattrarray(): %s\n", error, GRBgeterrormsg(env));
		  return 0;
	  }
	  else if (optimstatus == GRB_INF_OR_UNBD) {
	    printf("Model is infeasible or unbounded\n");
	    return 0;
	  }
	  return 1;
}

/* Chooses for every row and column, 1 value */

int choosevforij(GRBmodel *model,int N,GRBenv *env){
	int error=0,N3=(int)pow(N,3),*ind=(int*)calloc(N3,sizeof(int)),a,i,j;
	double    *val=(double *)calloc(N3,sizeof(double));
	  for(j=0;j<N3;j+=N){
		  for(i=0;i<N;i++){
			  a=i+j;
			  ind[i]=a;
			  val[i]=1;
		  }
		error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
		if (error) {
		printf("ERROR %d in 1st GRBaddconstr() number %d: %s\n", error,j, GRBgeterrormsg(env));
  		free(val);
  		free(ind);
			return 0;
		}
	  }
		free(val);
		free(ind);
	  return 1;
}

/* Chooses for every row and value, 1 column */

int choosejforiv(GRBmodel *model,int N,GRBenv *env){
	int error=0,N3=(int)pow(N,3),*ind=(int*)calloc(N3,sizeof(int)),a,i,j,k,N2=(int)pow(N,2);
	double    *val=(double *)calloc(N3,sizeof(double));
	for(k=0;k<N;k++){
		  for(j=0;j<N;j++){
			  for(i=0;i<N;i++){
				  a=j+i*N+k*N2;
				  ind[i]=a;
				  val[i]=1;
			  }
			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR %d in 2nd GRBaddconstr() number %d: %s\n", error,j+k, GRBgeterrormsg(env));
		  		free(val);
		  		free(ind);
				return 0;
			}

		  }
	  }
		free(val);
		free(ind);
	  return 1;
}

/* Chooses for every column and value, 1 row */

int chooseiforjv(GRBmodel *model,int N,GRBenv *env){
	int error=0,N3=(int)pow(N,3),*ind=(int*)calloc(N3,sizeof(int)),a,i,j,k,N2=(int)pow(N,2);
	double    *val=(double *)calloc(N3,sizeof(double));
	  for(k=0;k<N;k++){
		  for(j=0;j<N;j++){
			  for(i=0;i<N;i++){
				  a=i*N2+j+k*N;
				  ind[i]=a;
				  val[i]=1;

			  }
			error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
					  if (error) {
						  printf("ERROR %d in 3rd GRBaddconstr() number %d: %s\n", error,k+j, GRBgeterrormsg(env));
					  		free(val);
					  		free(ind);
						  return -1;
					  }

		  }
	  }
	free(val);
	free(ind);
	  return 1;
}

/* Sets 1 for every value that's ALREADY in the model as sent from previous functions */

int chooseijv(GRBmodel *model,int N,GRBenv *env){
	int error=0,N3=(int)pow(N,3),*ind=(int*)calloc(N3,sizeof(int)),i,N2=(int)pow(N,2);
	double    *val=(double *)calloc(N3,sizeof(double));
	for(i=0;i<N3;i++){
		  if(array[(int)floor(i/N2)][(int)floor((i%N2)/N)]==(i%N+1)){
			  ind[0]=i;
			  val[0]=1;
			  error = GRBaddconstr(model, 1, ind, val, GRB_EQUAL, 1.0, NULL);
			  if (error) {
			  	printf("ERROR %d in 4th GRBaddconstr() number %d: %s\n", error,i, GRBgeterrormsg(env));
		  		free(val);
		  		free(ind);
			  	return 0;
			  }
		  }
	  }
	free(val);
	free(ind);
	return 1;
}

/* Chooses values for each BLOCK */

int chooseBlock(GRBmodel *model,int N,GRBenv *env){
	int error=0,N3=(int)pow(N,3),*ind=(int*)calloc(N3,sizeof(int)),a,i,j,k,N2=(int)pow(N,2),J,I,b;
	double    *val=(double *)calloc(N3,sizeof(double));
	for(k=0;k<N;k++){
		  for(I=1;I<=m;I++){
			  for(J=1;J<=n;J++){
				  b=0;
				  for(j=0;j<m;j++){
					  for(i=0;i<n;i++){
						  a=(i*N+j*N2+(I-1)*N*n+(J-1)*N2*m)+k;
						  ind[b]=a;
						  val[b]=1;
						  b++;
					  }
				  }
				  error = GRBaddconstr(model, N, ind, val, GRB_EQUAL, 1.0, NULL);
				  if (error) {
				  		printf("ERROR %d in 5th GRBaddconstr() number %d: %s\n", error,i+j, GRBgeterrormsg(env));
				  		free(val);
				  		free(ind);
				  		return 0;
				  }
			  }
		  }
	  }
	free(val);
	free(ind);
	return 1;
}

/* This function prints the board the user sees */

void printB(){
	int i,j,k;
	for(i=0;i<(m*n);i++){
		if(i%(m)==0){
			for(k=0;k<((4*n*m)+m+1);k++){
				printf("%s","-");
			}
			printf("\n");
		}
	for(j=0;j<(m*n);j++){
					if(j%(n)==0){
						printf("|");
					}
					if((markError||mode==editMode)&&astrex[i][j]&&!fixed[i][j]){
						if(!array[i][j]){
							printf("    ");
						}
						else{
							printf(" %2d*",array[i][j]);
						}
					}
					else{
						if(!array[i][j]){
							printf("    ");
						}
						else{
							if(fixed[i][j]){
								printf(" %2d.",array[i][j]);
							}
							else{
									printf(" %2d ",array[i][j]);
								}
						}
					}
					if(j==(m*n)-1){
						printf("|\n");
					}
				}
				if(i==(m*n)-1){
					for(k=0;k<((4*n*m)+m+1);k++){
						printf("%s","-");
					}
					printf("\n");
				}
			}
	return;
}

/* Generates X random cells */

bool generateX(int x){
	int r,c,j,i,k,N3=(int)pow(n*m,3),N2=(int)pow(n*m,2);
	int **marked,*temp1,*legal;
	marked =(int **)calloc(n*m,sizeof(int*));
	temp1=(int *)calloc(N2,sizeof(int));
	for (i=0; i<m*n; i++){
		  marked[i] = (int *)calloc(n*m,sizeof(int));
	}
	legal=(int *)calloc(N3,sizeof(int));
	legal[0]=0;
	arraysZeroing(marked,1,1);
	i=0;
	while(i<2*x){						/** chooses X random cells **/
		r=rand()%(n*m);
		c=rand()%(n*m);
		if(marked[r][c]==0){
			marked[r][c]=1;
			temp1[i]=r;
			temp1[i+1]=c;
			i+=2;
		}
	}
	for(i=0;i<2*x;i+=2){	/** fills the x cells **/
		r=temp1[i];
		c=temp1[i+1];
		k=0;
		for(j=1;j<(n*m)+1;j++){	/** check all the legal values for the specific cell (k is the number of legal values) **/
			if(setSolver(r,c,j)){
				legal[k]=j;
				k++;
			}
		}
		if(k!=0){
			 j=rand()%k;
			 array[r][c]=legal[j];
			 for(j=0;j<m*n;j++){
				 legal[j]=0;
			 }
		}
		else{
			if(C==1000){
				freeGenerate(marked,temp1,legal);
				return 0;
			}
			C++;
			generateX(x);
			freeGenerate(marked,temp1,legal);
			return 0;
		}
	}
	freeGenerate(marked,temp1,legal);
	return 1;
}

/* Frees the arrays in GenerateX */

void freeGenerate(int** a,int* b,int* c){
	freeMem(a);
	free(b);
	free(c);
}
/* meapeset the arrays in GenerateX, with flags pointing to which arrays to zero */

void arraysZeroing(int** marked,bool toZeroArray,bool toZeroMarked){
	int i,j;
	for(i=0;i<n*m;i++){
		for(j=0;j<n*m;j++){
			if(toZeroMarked){
				marked[i][j]=0;}
			if(toZeroArray){
				array[i][j]=0;}
		}
	}
}

/* Removes all but Y cells */

void removeY(int y){
	int r,c,i=0,N2=n*m*n*m,N=n*m;
	while(i<(N2-y)){						/** chooses X random cells **/
		r=rand()%N;
		c=rand()%N;
		if(array[r][c]!=0){
			array[r][c]=0;
			i++;
		}
	}
	for(r=0;r<m*n;r++){
		for(c=0;c<m*n;c++){
			if(array[r][c]!=0){
				append(&listPointer,"generate", c+1, r+1,array[r][c], 0);
			}
		}
	}
}

/* Initializes a new board (GenerateXY) */

bool initialize(int x,int y){
	int i,j,N3=(int)pow(n*m,3),N2=(int)pow(n*m,2),N=n*m;
	double *sol=(double *)calloc(N3,sizeof(double));
	for(i=0;i<n*m;i++){
		for(j=0;j<n*m;j++){
			array[i][j]=0;
		}
	}
	i=0;
	C=0;
	while(i==0&&C<1000){	/* trying to generate x cells and solving the board */
		generateX(x);
		i=ILPFunction(sol);
		C++;
	}
	if(C==1000){
		free(sol);
		return 0;
	}
	for(i=0;i<N3;i++){	/* mapping the solution found by the ILPFunction to a 2D board */
		if(sol[i]!=0){
			array[(int)floor(i/N2)][(int)floor((i%N2)/(n*m))]=(i%(N)+1);
		}
	}
	if(listPointer!=NULL){
		while(listPointer->next!=NULL){ /*deleting the commands that were 'undo'*/
			 deleteNode(&listPointer, listPointer->next);
	 	}
	}
	removeY(y);
	free(sol);
	return 1;
}
/* Gives the user a hint to the cell x,y */

int getHint(int x,int y){
	int i=0,N3=(int)pow(n*m,3),N2=(int)pow(n*m,2),N=n*m;
	double *sol=(double *)calloc(N3,sizeof(double));
	i=ILPFunction(sol);
	if(!i){
		return 0;
	}
	for(i=0;i<N3;i++){	/* mapping the solution found by the ILPFunction to a 2D board */
		  if(x==((int)floor(i/N2))&&(y==(int)floor((i%N2)/N))){
			if(sol[i]){
				free(sol);
				return (i%N+1);
			}
		}
	}
	free(sol);
	return 0;
}

/* Checks if the board is valid */

bool isValid(){
	double *sol=(double *)calloc(n*m*n*m*n*m,sizeof(double));
	int i;
	i=ILPFunction(sol);
	free(sol);
	return i;
}

/* Checks if its legal to set v in the cell i,j */

bool setSolver(int i,int j,int v){
	int l,k,checkRow,checkColumn;
			for(k=0;k<m*n;k++){
				if(array[i][k]==v){
					return 0;
				}
			}
			for(k=0;k<m*n;k++){
				if(array[k][j]==v){
					return 0;
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
									if(array[k][l]==v && v!=0 && ((k!=i) || (l!=j))){
										if(array[k][l]==v){
											return 0;
										}
									}
								}
							}
						}
						checkColumn++;
					}
				}
				checkRow++;
			}
	return 1;
}

/* push row,column into the stack */

void push(int row,int coloumn)
{
   StackNode *newNode;
   newNode = (struct StackNode*)malloc(sizeof(struct StackNode));
   newNode->row = row;
   newNode->coloumn=coloumn;
   newNode->next = top;
   top = newNode;
}

/* pops last thing in stack */

void pop()
{
      StackNode* temp;
   if(top == NULL)
      return;
      temp= top;
      top = temp->next;
      free(temp);
}

/* Checks if the stack is empty */

bool isEmpty(){
    return top == NULL ? 1 : 0;
}

/* Find the number of solutions in the current board */

int solverNumOfSolutions(){
	int i,j,c=0;
	int** temp;
	temp =(int **)calloc(n*m,sizeof(int *));
	for (i=0; i<m*n; i++){
		temp[i] = (int *)calloc(n*m,sizeof(int));
	}
	for(i=0;i<n*m;i++){
		for(j=0;j<n*m;j++){
			temp[i][j]=array[i][j];
		}
	}
	push(0,0);
	while(!isEmpty(top)){
		i=top->row;
		j=top->coloumn;
		if(j==(n*m)-1){
			if(i==(n*m)-1){
				pop();
				c++;
				continue;
			}
			else{
				if(temp[i][j]){
					pop();
					push(i+1,0);
					continue;
				}
				else{
					if(loop1(i,j)){
						continue;}
					array[i][j]=0;
					pop();
				}
			}
		}
		else{
			if(temp[i][j]){
				pop();
				push(i,j+1);
				continue;
			}
			else{
				if(loop1(i,j)){
					continue;}
			array[i][j]=0;
			pop();
			}
		}
	}
	for(i=0;i<n*m;i++){
		for(j=0;j<n*m;j++){
			array[i][j]=temp[i][j];
		}
	}
	freeMem(temp);
	return c;
}

/* Checks which value is legal to set in i,j sets it and pushes to the stack */

bool loop1(int i,int j){
	int k;
	for(k=array[i][j]+1;k<((n*m)+1);k++){
		if(setSolver(i,j,k)){
			array[i][j]=k;
			if(j==(n*m)-1){
				push(i+1,0);
			}
			else{
				push(i,j+1);
			}
			return 1;
		}
	}
	return 0;
}


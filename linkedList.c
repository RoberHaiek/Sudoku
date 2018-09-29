#include"linkedList.h"
#include"Solver.h"
#include"Game.h"
#include<stdbool.h>
#include"Parser.h"
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

extern int isItFirstNode;

void append(struct Node** head_ref, char *command,int X,int Y,int newZ,int oldZ)
{
    /* 1. allocate node */
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));	
    struct Node* first_node = (struct Node*)malloc(sizeof(struct Node));/*add*/
  
 	
   struct Node* last = *head_ref; /* used in step 5*/
   

    /* 2. put in the data  */
    new_node->command = command;
    new_node->X =X;
    new_node->Y =Y;
    new_node->newZ =newZ;
    new_node->oldZ =oldZ;

    /*add*/
 
    first_node->command ="firstElement";
    first_node->X =X;
    first_node->Y =Y;
    first_node->newZ =newZ;
    first_node->oldZ =oldZ;
    

    /* 3. This new node is going to be the last node, so
          make next of it as NULL*/
    new_node->next = NULL;

    /*this going to be first_node prev*/
     if(isItFirstNode){
     first_node->prev=NULL;
     }

    /* 4. If the Linked List is empty, then make the new
          node as head */
    if (*head_ref == NULL) {
/*        new_node->prev = NULL;
        *head_ref = new_node;
        return;*/
    	new_node->prev=first_node;
    	new_node->prev->next=new_node;
       /* new_node->prev =*head_ref;make new_node the next of *head_ref(wich is NULL)
        new_node->prev->next=new_node;*/
        *head_ref = new_node;
        return;
    }

    /* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;

    /* 6. Change the next of last node */
    last->next = new_node;

    /* 7. Make last node as previous of new node */
    new_node->prev = last;
    *head_ref=new_node;
    if(!isItFirstNode){
	free(first_node);
    }
    
    isItFirstNode=0;
    return;
}

/* Function to delete a node in a Doubly Linked List.
   head_ref --> pointer to head node pointer.
   del  -->  pointer to node to be deleted. */

void deleteNode(struct Node **head_ref, struct Node *del)
{
  /* base case */
  if(*head_ref == NULL || del == NULL)
    return;

  /* If node to be deleted is head node */
  if(*head_ref == del)
    *head_ref = del->next;

  /* Change next only if node to be deleted is NOT the last node */
  if(del->next != NULL)
    del->next->prev = del->prev;

  /* Change prev only if node to be deleted is NOT the first node */
  if(del->prev != NULL)
    del->prev->next = del->next;

  /*moving pointer to end of the list*/
/*  while((*head_ref)->next !=NULL){
	  *head_ref=(*head_ref)->next;
  }*/

  /* Finally, free the memory occupied by del*/
  free(del);
  return;
}

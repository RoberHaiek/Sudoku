/*
 * linkedList.h
 *
 *  Created on: Sep 10, 2018
 *      Author: Weaam
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_



#endif /* LINKEDLIST_H_ */
/* Node of a doubly linked list to keep track of the undo/redo */

typedef struct Node  {
    char *command;
    int X,Y,newZ,oldZ;
    struct Node* next; /* Pointer to next node in DLL*/
    struct Node* prev; /* Pointer to previous node in DLL*/
}Node;

void deleteNode(struct Node **head_ref, struct Node *del);/* deletes a node from the linked list */
void append(struct Node** head_ref, char *command,int X,int Y,int newZ,int oldZ);/* appends a node to the linked list */


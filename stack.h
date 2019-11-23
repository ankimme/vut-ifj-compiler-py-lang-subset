#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>

#define MAX_STACK 20

typedef struct 
{
	int arr[MAX_STACK];                            
	int top;                                
} tStack;

void stackInit ( tStack* s );
int stackEmpty ( const tStack* s );
int stackFull ( const tStack* s );
void stackTop ( const tStack* s, int* c );
void stackPop ( tStack* s );
void stackPush ( tStack* s, int c );

#endif


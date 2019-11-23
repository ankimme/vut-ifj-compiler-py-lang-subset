#include "stack.h"

int solved;
int STACK_SIZE = MAX_STACK;

void stackInit ( tStack* s ) {

   if (s == NULL){
       stackError(SERR_INIT);
       return;
   }

   s->top = -1;

}

int stackEmpty ( const tStack* s ) {

    return s->top == -1;
}

int stackFull ( const tStack* s ) {

    return s->top == STACK_SIZE - 1;   
}

void stackTop ( const tStack* s, int* i ) {

    if (stackEmpty(s)){
        return;
    }

    *i = s->arr[s->top];
}


void stackPop ( tStack* s ) {

    if (!stackEmpty(s)){
        s->top--;
}


void stackPush ( tStack* s, int i ) {


    if (stackFull(s)){
        return;
    }

    s->top++;  
    s->arr[s->top] = i;

}

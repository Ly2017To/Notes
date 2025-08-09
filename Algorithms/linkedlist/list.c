#include <stdio.h>
#include <stdlib.h>
#include "list.h"

// create a new node;
node_t * newNode(int value)
{
    node_t * p = (node_t *)malloc(sizeof(node_t));

    if(p==NULL){
        printf("malloc error\n");
        exit(FAILURE);
    }

    p->value=value;
    p->next=NULL;

    return p;
}

// push a node into the head of the list;
node_t * push(node_t * pHead, node_t * p)
{
    if(pHead==NULL){
        pHead=p;
    }else{
        p->next=pHead;
        pHead=p;
    }

    return pHead;
}

// pop the head node of the list;
node_t * pop(node_t * pHead)
{
    node_t * pTmp=NULL;

    if(pHead==NULL){
        printf("this linked list is empty\n");
        return pHead;
    }

    if(pHead->next==NULL){
        printf("no pop if there is only one node in the list\n");
    }else{
        pTmp=pHead;
        pHead=pHead->next;
        free(pTmp);
    }

    return pHead;
}

// check whether a specific value is in the list;
void checkValue(node_t * pHead, int value)
{
    while(pHead!=NULL){
        if(pHead->value==value){
            break;
        }
        pHead=pHead->next;
    }

    if(pHead==NULL){
         printf("value %d is not in the list\n",value);
    }else{
         printf("value %d is in the list\n",value);
    }

    return;
}

// delete a node with a specific value;
node_t * deleteNode(node_t * pHead, int value)
{
    node_t *current, *previous, *pTmp;

    current=pHead;
    previous=pTmp=NULL;

    if(pHead==NULL){
        return pHead;
    }

    while(current!=NULL){
        if(current->value==value){
           break;
        }
        previous=current;
        current=current->next;
    }

    if(current==pHead){
        pTmp=pHead;
        pHead=pHead->next;
        free(pTmp);
    }else{
        pTmp=current;
        previous->next=current->next;
        free(pTmp);
    }

    return pHead;
}

// reverse the linked list;
node_t * reverseList(node_t * pHead)
{
    node_t *pNext, *pPre;

    pNext=pPre=NULL;

    while(pHead!=NULL){
        pNext=pHead->next;
        pHead->next=pPre;
        pPre=pHead;
        pHead=pNext;
    }

    return pPre;
}

// print the lined list;
void printList(node_t * pHead)
{
    printf("\nprint:");
    while(pHead!=NULL){
        printf("%d",pHead->value);
        pHead=pHead->next;
        if(pHead!=NULL){
            printf("->");
        }
    }
    printf("\n");
    return;
}

// free the heap memory of the list;
void freeList(node_t * pHead)
{
    node_t * pTmp=NULL;

    while(pHead!=NULL){
        pTmp=pHead;
        pHead=pHead->next;
        free(pTmp);
    }

    return;
}

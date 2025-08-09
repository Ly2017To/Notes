/*
    linked list(LIFO) contains the following operations
    1. create a new node;
    2. push a node into the head of the list;
    3. pop the head node of the list;
    4. check whether a specific value is in the list;
    5. delete a node with a specific value;
    6. reverse the linked list;
    7. print the lined list;
    8. free the heap memory of the list;
*/

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main()
{
    //five nodes with value 0,1,2,3,4
    node_t * p0 = newNode(0);
    node_t * p1 = newNode(1);
    node_t * p2 = newNode(2);
    node_t * p3 = newNode(3);
    node_t * p4 = newNode(4);

    //head pointer of the linked list
    node_t * pHead = NULL;
    pHead=push(pHead,p0);
    pHead=push(pHead,p2);
    pHead=push(pHead,p4);
    pHead=push(pHead,p1);
    pHead=push(pHead,p3);
    printList(pHead); //print:3->1->4->2->0
    pHead=pop(pHead);
    printList(pHead); //print:1->4->2->0
    checkValue(pHead,3);
    checkValue(pHead,1);
    pHead=deleteNode(pHead,4);
    printList(pHead); //print:1->2->0
    pHead=reverseList(pHead);
    printList(pHead); //print:0->2->1

    //free the linked list
    freeList(pHead);

    return SUCCESS;
}

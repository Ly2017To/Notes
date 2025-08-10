#ifndef LIST_H
#define LIST_H

#define SUCCESS 0
#define FAILURE 1

typedef struct node{
    int value;
    struct node *next;
}node_t;

node_t * newNode(int);
node_t * push(node_t *, node_t *);
node_t * pop(node_t *);
void checkValue(node_t *, int);
node_t * deleteNode(node_t *, int);
node_t * reverseList(node_t *);
void printList(node_t *);
void freeList(node_t *);


#endif // LIST_H


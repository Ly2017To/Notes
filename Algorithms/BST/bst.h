/*
 * bst.h
 *
 *  Created on: Aug 29, 2023
 *      Author: luyuan
 */

#ifndef BST_H_
#define BST_H_

#define SUCCESS 0
#define FAILURE 1

typedef struct node{
	int key;
	struct node * left;
	struct node * right;
}node_t;

node_t * newNode(int);
node_t * insert(node_t *,int);
void inorder(node_t *);
void preorder(node_t *);
void postorder(node_t *);
int searchBST(node_t *,int);
int sizeBST(node_t *);
int depthBST(node_t *);
int getMaxBST(node_t *);
int getMinBST(node_t *);
node_t * deleteKeyBST(node_t *, int);
void freeBST(node_t *);


#endif /* BST_H_ */

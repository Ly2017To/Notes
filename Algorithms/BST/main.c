/*
 * main.c
 *
 *  Created on: Aug 29, 2023
 *      Author: luyuan
 */

/*
    bst(binary search tree) contains the following operations
    1. create a new node;
    2. insert a node;
    3. visit inorder: left subtree; root; right subtree;
    4. visit preorder: root; left subtree; right subtree;
    5. visit postorder: left subtree; right subtree; root;
    6. search whether a specific value is in the bst;
    7. size of the bst (the number of nodes);
    8. depth of the bst;
    9. maximum key of the bst;
    10. minimum key of the bst;
    11. delete a node with a specific value;
    12. free the heap memory of the bst;

 */

#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

int main()
{
  //buid a bst
  node_t * root;
  root=insert(root,5);
  root=insert(root,3);
  root=insert(root,9);
  root=insert(root,1);
  root=insert(root,4);
  root=insert(root,6);

  //visit inorder 1 3 4 5 6 9
  printf("visit inorder: ");
  inorder(root);
  printf("\n");

  //visit preorder
  //printf("visit preorder: ");
  //preorder(root);
  //printf("\n");

  //visit postorder
  //printf("visit postorder: ");
  //postorder(root);
  //printf("\n");

  //search a key in a BST
  int key=4;
  if(searchBST(root,key)){
    printf("find %d in the BST\n",key);
  }else{
    printf("NOT find %d in the BST\n",key);
  }

  //the number of nodes in a BST
  int size=sizeBST(root);
  printf("the size of BST is %d\n",size);

  //the depth of a BST
  int depth=depthBST(root);
  printf("the depth of BST is %d\n",depth);

  //the minimal key of a BST
  int max=getMaxBST(root);
  printf("the maximal key of BST is %d\n",max);

  //the minimal key of a BST
  int min=getMinBST(root);
  printf("the minimal key of BST is %d\n",min);

  //delete a node of a key in a BST
  root=deleteKeyBST(root, 5);
  printf("visit inorder: ");
  inorder(root);
  printf("\n");

  //free the heap memory
  freeBST(root);

  return SUCCESS;
}

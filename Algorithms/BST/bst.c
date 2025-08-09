/*
 * bst.c
 *
 *  Created on: Aug 29, 2023
 *      Author: luyuan
 */

#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

node_t * newNode(int key)
{
  node_t *p = (node_t *)malloc(sizeof(node_t));

  if(p==NULL){
    printf("malloc error\n");
    exit(FAILURE);
  }

  p->key=key;
  p->left=NULL;
  p->right=NULL;

  return p;
}

node_t * insert(node_t * node, int key)
{
  if(node==NULL){
    return (newNode(key));
  }

  if(key<node->key){
    node->left=insert(node->left,key);
  }else{
    node->right=insert(node->right,key);
  }

  return node;
}

void inorder(node_t * node)
{
  if(node==NULL){
    return;
  }

  inorder(node->left);
  printf("%d ",node->key);
  inorder(node->right);
}

void preorder(node_t * node)
{
  if(node==NULL){
    return;
  }

  printf("%d ",node->key);
  preorder(node->left);
  preorder(node->right);
}

void postorder(node_t * node)
{
  if(node==NULL){
    return;
  }

  postorder(node->left);
  postorder(node->right);
  printf("%d ",node->key);
}

int searchBST(node_t * node, int key)
{
  if(node==NULL){
    return 0;
  }

  if(node->key==key){
    return 1;
  }

  int find=0;

  if(node->key>key){
    find=searchBST(node->left, key);
  }else{
    find=searchBST(node->right, key);
  }

  return find;
}

int sizeBST(node_t * node)
{
  if(node==NULL){
    return 0;
  }

  return sizeBST(node->left)+sizeBST(node->right)+1;
}

int depthBST(node_t * node)
{
  if(node==NULL){
    return 0;
  }

  int leftDepth, rightDepth;

  leftDepth=depthBST(node->left);
  rightDepth=depthBST(node->right);

  if(leftDepth>rightDepth){
    return leftDepth+1;
  }
  return rightDepth+1;
}

int getMaxBST(node_t *node)
{
  node_t *pTmp = node;

  while(pTmp->right!=NULL){
    pTmp=pTmp->right;
  }

  return pTmp->key;
}

int getMinBST(node_t * node)
{
  node_t *pTmp = node;

  while(pTmp->left!=NULL){
    pTmp=pTmp->left;
  }

  return pTmp->key;
}

node_t * deleteKeyBST(node_t * node, int key)
{
  int leftMax, rightMin;

  if(node==NULL){
    return NULL;
  }

  if(node->key==key){
    if(node->left==NULL&&node->right==NULL){
      free(node);
      node=NULL;
      return node;
    }
    if(node->left!=NULL){
      leftMax=getMaxBST(node->left);
      node->key=leftMax;
      node->left=deleteKeyBST(node->left, leftMax);
    }else{
      rightMin=getMinBST(node->right);
      node->key=rightMin;
      node->right=deleteKeyBST(node->right, rightMin);
    }
  }

  if(node->key>key){
    node->left=deleteKeyBST(node->left, key);
  }else{
    node->right=deleteKeyBST(node->right, key);
  }

  return node;
}

void freeBST(node_t *node){
  if(node==NULL){
    return;
  }

  freeBST(node->left);
  freeBST(node->right);
  free(node);

  return;
}

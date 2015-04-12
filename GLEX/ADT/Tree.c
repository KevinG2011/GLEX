//
//  Tree.c
//  GLEX
//
//  Created by 李佳 on 15-4-12.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "Tree.h"

void treeInit(Tree* ptree)
{
    ptree->root->left = ptree->root->right = NULL;
    ptree->size = 0;
}
bool treeIsEmpty(const Tree* ptree)
{
    return ptree == NULL || ptree->size == 0;
}
bool treeIsFull(const Tree* ptree)
{
    return ptree->size == MAXITEMS;
}
size_t treeItemCount(const Tree* ptree)
{
    return ptree->size;
}

static bool CompareItem(const Item* i1,const Item* i2)
{
    return false;
}

static void AddNode(const Node* root,const Node* newNode)
{
    
}

Node* MakeNode(const Item *item)
{
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->left = newNode->right = NULL;
        newNode->item = *item;
    }
    
    return newNode;
}

bool treeItemIn(const Tree* ptree,Item *item)
{
    return false;
}
bool treeAddItem(Tree* ptree,const Item *item)
{
    if (treeIsFull(ptree)) {
        return false;
    }
    
    
    
    return false;
}
bool treeDeleteItem(Tree* ptree,const Item *item)
{
    return false;
}

void Traverse(const Tree* ptree,void (*pfunc)(Item item))
{
    
}
void treeDeleteAll(Tree* ptree)
{
    
}
//
//  Tree.h
//  GLEX
//
//  Created by 李佳 on 15-4-12.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#ifndef __GLEX__Tree__
#define __GLEX__Tree__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXITEMS 10

typedef struct item {
    char pet_name[20];
    char pet_kind[20];
} Item;

typedef struct node {
    Item item;
    struct node *left;
    struct node *right;
} Node;

typedef struct tree {
    Node * root;
    size_t size;
} Tree;

void treeInit(Tree* ptree);
bool treeIsEmpty(const Tree* ptree);
bool treeIsFull(const Tree* ptree);
size_t treeItemCount(const Tree* ptree);
bool treeItemIn(const Tree* ptree,Item *item);
bool treeAddItem(Tree* ptree,const Item *item);
bool treeDeleteItem(Tree* ptree,const Item *item);
void Traverse(const Tree* ptree,void (*pfunc)(Item item));
void treeDeleteAll(Tree* ptree);


#endif /* defined(__GLEX__Tree__) */

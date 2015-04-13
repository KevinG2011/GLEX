//
//  Tree.c
//  GLEX
//
//  Created by 李佳 on 15-4-12.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "Tree.h"

typedef struct pair {
    Node * parent;
    Node * child;
} Pair;


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
static int CompareItem(const Item* i1,const Item* i2)
{
    return strcmp(i1->pet_name, i2->pet_name);
}

//在树中查找与指定Item名字相同的Node节点
static Pair SeekItem(const Tree* ptree,const Item * item)
{
    Pair look;
    look.parent = NULL;
    look.child = ptree->root;
    
    if (look.child == NULL) {
        return look;
    }
    
    while (look.child != NULL) {
        int result = CompareItem(&look.child->item, item);
        if (result > 0) {
            look.parent = look.child;
            look.child = look.child->left;
        }
        else if (result < 0) {
            look.parent = look.child;
            look.child = look.child->right;
        }
        else
            break;
    }
    
    return look;
}

static void AddNode(Node* root,Node* newNode)
{
    int result = CompareItem(&root->item,&newNode->item);
    if (result == 0) {
        puts("不应含有相同项");
        exit(EXIT_FAILURE);
    }
    
    if (result > 0) {
        if (root->left == NULL) {
            root->left = newNode;
        } else {
            AddNode(root->left, newNode);
        }
    }
    else if(result < 0)
    {
        if (root->right == NULL) {
            root->right = newNode;
        } else {
            AddNode(root->right, newNode);
        }
    }
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

bool treeItemIn(const Tree* ptree,const Item *item)
{
    return SeekItem(ptree, item).child != NULL;
}

bool treeAddItem(Tree* ptree,const Item *item)
{
    //tree是否已经满了
    if (treeIsFull(ptree)) {
        fprintf(stderr, "Tree is full\n");
        return false;
    }
    //搜索item是否已经在树中
    if (treeItemIn(ptree, item)) {
        fprintf(stderr, "Attempted to add duplicate item\n");
        return false;
    }
    
    //创建
    Node* newNode = MakeNode(item);
    if (newNode == NULL) {
        fprintf(stderr, "Couldn't create node\n");
        return false;
    }
    
    if (ptree->root == NULL) {
        ptree->root = newNode;
    } else {
	    AddNode(ptree->root, newNode);
    }

    ptree->size++;
    return true;
}

static void DeleteNode(Node** pNode)
{
    if (pNode == NULL) {
        return;
    }
    printf("pNode %s",(*pNode)->item.pet_name);
    
    Node *temp;
    if ((*pNode)->left == NULL) {
        temp = (*pNode);
        (*pNode) = (*pNode)->right;
        free(temp);
    } else if ((*pNode)->right == NULL) {
        temp = (*pNode);
        (*pNode) = (*pNode)->left;
        free(temp);
    }
	else
    {
        for (temp = (*pNode)->left; temp->right != NULL; temp = temp->right)
            continue;
        
        temp->right = (*pNode)->right;
        temp = *pNode;
        
        *pNode = (*pNode)->left;
        free(temp);
    }
}

bool treeDeleteItem(Tree* ptree,const Item *item)
{
    Pair look;
    look = SeekItem(ptree, item);
    if (look.child == NULL) {
        return false;
    }
    
    if (look.parent == NULL)
        DeleteNode(&ptree->root);
    else if (look.parent->left == look.child)
        DeleteNode(&look.parent->left);
    else
        DeleteNode(&look.parent->right);
    ptree->size--;
    return true;
}

static void InOrder(const Node* root,void (*pfunc)(Item item))
{
    if (root != NULL) {
        InOrder(root->left,pfunc);
        (*pfunc)(root->item);
        InOrder(root->right,pfunc);
    }
}

void treeTraverse(const Tree* ptree,void (*pfunc)(Item item))
{
    if (ptree != NULL) {
        InOrder(ptree->root,pfunc);
    }
}

static void DeleteAllNodes(Node* root)
{
    if (root != NULL) {
        Node *pright = root->right;
        DeleteAllNodes(root->left);
        free(root);
        DeleteAllNodes(pright);
    }
}

void treeDeleteAll(Tree* ptree)
{
    if (ptree != NULL) {
        DeleteAllNodes(ptree->root);
    }
    ptree->root = NULL;
    ptree->size = 0;
}




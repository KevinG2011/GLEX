//
//  Queue.h
//  GLEX
//
//  Created by Li Jia on 15/4/10.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#ifndef __GLEX__Queue__
#define __GLEX__Queue__

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAXQUEUE 10

typedef int Item;

typedef struct node {
    Item item;
    struct node* next;
} Node;

typedef struct queue {
    Node * front;
    Node * back;
    size_t count;
} Queue;


void initQueue(Queue* queue);
bool isFullQueue(Queue* queue);
bool isEmptyQueue(Queue* queue);
size_t itemCountQueue(Queue* queue);
bool itemEnQueue(Queue* queue,Item item);
bool DeQueue(Queue* queue);
void clearQueue(Queue* queue);

#endif /* defined(__GLEX__Queue__) */

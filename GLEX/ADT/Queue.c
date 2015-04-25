//
//  Queue.c
//  GLEX
//
//  Created by Li Jia on 15/4/10.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "Queue.h"

void initQueue(Queue* queue)
{
    if (queue == NULL) {
        return;
    }
        
    queue->front = queue->back = NULL;
    queue->count = 0;
}

bool isFullQueue(Queue* queue)
{
    return queue->count == MAXQUEUE;
}

bool isEmptyQueue(Queue* queue)
{
    return queue == NULL || queue->count == 0;
}

size_t itemCountQueue(Queue* queue)
{
    return queue == NULL ? 0 : queue->count;
}

bool itemEnQueue(Queue* queue,Item item)
{
    if (queue == NULL) {
        return false;
    }
    
    if (queue->count < MAXQUEUE){
	    Node* node = (Node*)malloc(sizeof(Node));
       	if (node == NULL) {
        	puts("Unable to allcate memory!\n");
            exit(1);
        }
        
        node->item = item;
        
        if (isEmptyQueue(queue)) {
            queue->front = queue->back = node;
            node->next = node;
        }
		else
        {
            queue->back->next = node;
            node->next = queue->front;
            queue->back = node;
        }
        
        queue->count++;
        
        return true;
    }
    else
    {
        Node* newFront = queue->front->next;
        queue->front->item = item;
        
        queue->back = queue->front;
        queue->front = newFront;
        
        return true;
    }
}

bool DeQueue(Queue* queue)
{
    if (isEmptyQueue(queue)) {
        return false;
    }
    
    if (queue->count == 1) {
        free(queue->front);
        queue->front = queue->back = NULL;
    }
    else
    {
        Node* newFront = queue->front->next;
        free(queue->front);
        queue->back->next = queue->front = newFront;
    }
    
    queue->count--;
    return true;
}

void clearQueue(Queue* queue)
{
    Node* node = queue->front;
    while (node != NULL) {
        Node *next = node->next;
        free(node);
        node = next;
    }
    
    queue->front = queue->back = NULL;
    queue->count = 0;
}


int queueMain(int argc,const char* argv[])
{
    Queue queue;
    Item tmp;
    char ch;
    
    initQueue(&queue);
    puts("Testing the Queue interface. Type a to add a value.");
    puts("type d to delete a value , and type q quit");
    while ((ch = getchar()) != 'q') {
        if (ch != 'a' && ch != 'd') {
            continue;
        }
        
        if (ch == 'a') {
            printf("Integer to add: ");
            scanf("%d",&tmp);
            itemEnQueue(&queue, tmp);
        }
        else
        {
            DeQueue(&queue);
        }
        
        printf("%zu items in queue\n",itemCountQueue(&queue));
        puts("Type a to add. d to delete. q to quit :");
    }
    
    clearQueue(&queue);
    puts("Bye!");
    
    return 0;
}


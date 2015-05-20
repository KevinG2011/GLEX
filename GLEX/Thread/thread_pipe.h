//
//  thread_pipe.h
//  GLEX
//
//  Created by 李佳 on 15-5-19.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#ifndef __GLEX__thread_pipe__
#define __GLEX__thread_pipe__

#include <stdio.h>
#include <pthread.h>
#include "errors.h"

typedef struct stage_tag {
    pthread_mutex_t mutex;
    pthread_cond_t avail;
    pthread_cond_t ready;
    int data_ready;
    long data;
    pthread_t thread;
    struct stage_tag *next;
} stage_t;

typedef struct pipe_tag {
    pthread_mutex_t mutex;
    stage_t *head;
    stage_t *tail;
    int stages;
    int active;
    
} pipe_t;

int pipe_send(stage_t *stage,long data);
void* pipe_stage(void* arg);
int pipe_create(pipe_t* pipe,int stages);
int pipe_start(pipe_t* pipe,long value);
int pipe_result(pipe_t* pipe,long* result);

int pipeMain(int argc, const char * argv[]);
#endif /* defined(__GLEX__thread_pipe__) */

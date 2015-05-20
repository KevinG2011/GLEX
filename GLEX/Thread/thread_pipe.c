//
//  thread_pipe.c
//  GLEX
//
//  Created by 李佳 on 15-5-19.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "thread_pipe.h"

int pipe_send(stage_t *stage,long data)
{
    int status = pthread_mutex_lock(&stage->mutex);
    if (status != 0) return status;
    
    while (stage->data_ready) {
        status = pthread_cond_wait(&stage->ready, &stage->mutex);
        if (status != 0) {
            pthread_mutex_unlock(&stage->mutex);
            return status;
        }
    }
    
    /*
     *  Send the new data
     */
    stage->data = data;
    stage->data_ready = 1;
    status = pthread_cond_signal(&stage->avail);
    if (status != 0) {
        pthread_mutex_unlock(&stage->mutex);
        return status;
    }
    status = pthread_mutex_unlock(&stage->mutex);
    return status;
}

void* pipe_stage(void* arg)
{
    stage_t *stage = (stage_t*)arg;
    stage_t *next_stage = stage->next;
    int status = pthread_mutex_lock(&stage->mutex);
    if (status != 0)
        err_abort(status, "Lock pipe stage");
    
    while (1) {
        //每个阶段都进入等待唤醒阶段,监听当前线程的数据是否准备完毕(data_ready = 1)
        while (stage->data_ready != 1) {
            status = pthread_cond_wait(&stage->avail, &stage->mutex);
            if (status != 0)
                err_abort(status, "Wait for previous stage");
        }
        pipe_send(next_stage, stage->data + 1);
        stage->data_ready = 0;
        status = pthread_cond_signal(&stage->ready);
        if (status != 0) {
            err_abort(status, "Wake next stage");
        }
    }
    
    return NULL;
}

int pipe_create(pipe_t* pipe,int stages)
{
    stage_t **link = &pipe->head;
    stage_t *new_stage = NULL;
    //初始化互斥量
    int status = pthread_mutex_init(&pipe->mutex, NULL);
    if (status != 0) {
        err_abort(status, "Init pipe mutex");
    }
    //保存步骤数量
    pipe->stages = stages;
    //处于活动状态
    pipe->active = 0;
    
    //将流水线各阶段初始化并链接起来
    for (int pipe_index = 0; pipe_index <= stages; pipe_index++) {
        new_stage = (stage_t*)malloc(sizeof(stage_t));
        if (new_stage == NULL)
            errno_abort("Allocate stage error");
        //初始化步骤互斥量
        status = pthread_mutex_init(&new_stage->mutex,NULL);
        if (status != 0) {
            err_abort(status, "Init stage mutex");
        }
        //初始化可用条件变量
        status = pthread_cond_init(&new_stage->avail, NULL);
        if (status != 0) {
            err_abort(status, "Init avail condition");
        }
        //初始化准备条件变量
        status = pthread_cond_init(&new_stage->ready, NULL);
        if (status != 0) {
            err_abort(status, "Init ready condition");
        }
        new_stage->data_ready = 0;
        *link = new_stage;
        link = &new_stage->next;
    }
    
    *link = NULL;
    //将流水线最后一个阶段保存为尾端.
    pipe->tail = new_stage;
    
    /*
     *  在所有步骤数据初始化完毕后,pipe_create为每一步建立一个线程.额外的"最后一步"没有对应的线程.
     *  for循环的终止条件是当前步骤的next指针为空,不会处理最后一步
     */
    for (stage_t* stage = pipe->head; stage->next != NULL; stage = stage->next) {
        status = pthread_create(&stage->thread, NULL, pipe_stage, (void*)stage);
        if (status != 0)
            err_abort(status, "Create pipe stage");
    }
    return status;
}

int pipeMain(int argc, const char * argv[])
{
    pipe_t my_pipe;
    long value,result;
    int status;
    char line[128];
    
    //初始化一个指定截断数量的流水线,此处为10
    pipe_create(&my_pipe, 3);
    while (1) {
        printf("Data> ");
        if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
        if (strlen(line) <= 1) continue;
        if (strlen(line) <= 2 && line[0] == '=') {
            if (pipe_result(&my_pipe, &result))
                printf("Result is %ld\n",result);
            else
                printf("Pipe is empty\n");
        } else {
            if (sscanf(line, "%ld",&value) < 1)
                fprintf(stderr, "Enter an integer value\n");
            else
                //从命令行读取数值,启动流水线
                pipe_start(&my_pipe, value);
        }
    }
    
    return status;
}

int pipe_start(pipe_t* pipe,long value)
{
    int status = pthread_mutex_lock(&pipe->mutex);
    if (status != 0)
        err_abort(status, "Lock pipe mutex");
    //激活pipe流水线
    pipe->active++;
    status = pthread_mutex_unlock(&pipe->mutex);
    if (status != 0)
        err_abort(status, "Unlock pipe mutex");
    //启动流水线
    pipe_send(pipe->head, value);
    return 0;
}

int pipe_result(pipe_t* pipe,long* result)
{
    stage_t* tail = pipe->tail;
    long value;
    int empty = 0;
    
    int status = pthread_mutex_lock(&pipe->mutex);
    if (status != 0)
        err_abort(status, "Lock pipe mutex");
    if (pipe->active <= 0)
        empty = 1;
    else
        pipe->active--;
    
    status = pthread_mutex_unlock(&pipe->mutex);
    if (status != 0)
        err_abort(status, "Unlock pipe mutex");
    if (empty) return 0;
    
    pthread_mutex_lock(&tail->mutex);
    while (!tail->data_ready) {
        pthread_cond_wait(&tail->avail, &tail->mutex);
    }
    *result = tail->data;
    tail->data_ready = 0;
    pthread_cond_signal(&tail->ready);
    pthread_mutex_unlock(&tail->mutex);
    return 1;
}








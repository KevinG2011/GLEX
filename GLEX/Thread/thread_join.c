//
//  thread_join.c
//  GLEX
//
//  Created by Li Jia on 15/4/24.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "thread_join.h"

typedef struct my_struct_tag {
    pthread_mutex_t mutex;
    int value;
} my_struct_t;

my_struct_t data = { PTHREAD_MUTEX_INITIALIZER , 0 };

void* thread_routine(void * arg)
{
    int status;
    my_struct_t *data = (my_struct_t*)arg;
    status = pthread_mutex_lock(&data->mutex);
    if (status != 0 ) err_abort(status, "Lock mutex");
    
    data->value = 3;
    printf("routine sleep time (%d)",data->value);
    sleep(data->value);
    
    status = pthread_mutex_unlock(&data->mutex);
    if (status != 0 ) err_abort(status, "Unlock mutex");

    return arg;
}

int joinMain(int argc, const char * argv[])
{
    pthread_t thread;
    int status;
    my_struct_t *data = (my_struct_t*)malloc(sizeof(my_struct_t));
    if (data == NULL) errno_abort("Allocate structure");
    
    status = pthread_mutex_init(&data->mutex, NULL);
    if (status != 0 ) err_abort(status, "Init mutex");
    
    status = pthread_create(&thread, NULL, thread_routine, data);
    if (status != 0 ) err_abort(status, "Create pthread");
    
    status = pthread_join(thread, NULL);
    if (status != 0 ) err_abort(status, "Join pthread");
    
    status = pthread_mutex_destroy(&data->mutex);
    if (status != 0) err_abort(status, "Destory mutext");
    free(data);
    
    return status;
}


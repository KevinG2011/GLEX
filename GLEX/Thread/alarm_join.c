//
//  alarm_join.c
//  GLEX
//
//  Created by Li Jia on 15/4/24.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "alarm_join.h"

typedef struct my_struct_tag {
    pthread_mutex_t mutex;
    int value;
} my_struct_t;

my_struct_t data = { PTHREAD_MUTEX_INITIALIZER , 0 };


int joinMain(int argc, const char * argv[])
{
    my_struct_t *data;
    int status;
    data = (my_struct_t*)malloc(sizeof(my_struct_t));
    if (data == NULL) errno_abort("Allocate structure");
    
    status = pthread_mutex_init(&data->mutex, NULL);
    if (status != 0 )
        err_abort(status, "Init mutex");
    status = pthread_mutex_destroy(&data->mutex);
    if (status != 0)
        err_abort(status, "Destory mutext");
    free(data);
    return status;
}
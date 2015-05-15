//
//  alarm_cond.c
//  GLEX
//
//  Created by Li Jia on 15/4/24.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "alarm_cond.h"
typedef struct my_struct_tag {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int value;
} my_struct_t ;

my_struct_t condData = { PTHREAD_MUTEX_INITIALIZER,PTHREAD_COND_INITIALIZER, 0};

int condMain(int argc, const char * argv[])
{
    int status;
    my_struct_t * data = (my_struct_t*)malloc(sizeof(my_struct_t));
    if (data == NULL) errno_abort("Allocate structure");
    
    status = pthread_mutex_init(&condData.mutex, NULL);
    if (status != 0)
        err_abort(status, "Init mutex");
    
    status = pthread_cond_init(&condData.cond, NULL);
    if (status != 0)
        err_abort(status, "Init cond");
    
    status = pthread_mutex_destroy(&condData.mutex);
    if (status != 0)
        err_abort(status, "Destroy mutex");

    status = pthread_cond_destroy(&condData.cond);
    if (status != 0)
        err_abort(status, "Destroy cond");
    
    free(data);
    
    return status;
}
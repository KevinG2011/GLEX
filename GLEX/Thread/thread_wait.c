//
//  thread_wait.c
//  GLEX
//
//  Created by 李佳 on 15-5-14.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "thread_wait.h"
typedef struct thread_wait_data {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int value;
} WaitData;

WaitData waitData = { PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, 0 };

int hibernation = 1;

void* wait_thread(void* arg)
{
    int status;
    sleep(hibernation);
    status = pthread_mutex_lock(&waitData.mutex);
    if (status != 0 ) err_abort(status, "Lock Mutex");
    waitData.value = 1;
    status = pthread_cond_signal(&waitData.cond);
    if (status != 0 ) err_abort(status, "Signal condition");
    status = pthread_mutex_unlock(&waitData.mutex);
    if (status != 0 ) err_abort(status, "Unlock Mutex");
    return NULL;
}

int waitMain(int argc, const char * argv[])
{
    int status;
    pthread_t w_thread;
    struct timespec timeout;
    if (argc > 1) hibernation = atoi(argv[1]);
    
    status = pthread_create(&w_thread, NULL,wait_thread, NULL);
    if (status != 0 ) err_abort(status, "Create Wait thread");
    timeout.tv_sec = time(NULL) + 2;
    timeout.tv_nsec = 0;
    
    status = pthread_mutex_lock(&waitData.mutex);
    if (status != 0 ) err_abort(status, "Lock Mutex");
    
    while (waitData.value == 0) {
        status = pthread_cond_timedwait(&waitData.cond, &waitData.mutex,&timeout);
        if (status == ETIMEDOUT) {
            printf("Condition wait timed out. \n");
            break;
        }
        else if (status != 0 )
            err_abort(status, "Wait on condition");
    }
    
    if (waitData.value != 0 )
        printf("Condition was signaled.\n");
    status = pthread_mutex_unlock(&waitData.mutex);
    if (status != 0 ) err_abort(status, "Unlock Mutex");
    
    return status;
}








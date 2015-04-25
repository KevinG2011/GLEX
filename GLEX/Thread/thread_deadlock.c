//
//  thread_deadlock.c
//  GLEX
//
//  Created by Li Jia on 15/4/25.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "thread_deadlock.h"
#define ITERATIONS 10

pthread_mutex_t mutexs[3] = {
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER,
    PTHREAD_MUTEX_INITIALIZER
};

int backoff = 1;
int yield_flag = 2;

void* lock_forward(void* arg) {
    int i,iterate,backoffs;
    int status;
    
    for (iterate = 0; iterate < ITERATIONS; iterate++) {
        backoffs = 0;
        for (i = 0; i < 3; i++) {
            if (i == 0) {
				status = pthread_mutex_lock(&mutexs[i]);
                if (status != 0 ) {
					err_abort(status, "Lock backward thread");
                }
            } else {
                if (backoff)
                    status = pthread_mutex_trylock(&mutexs[i]);
                else
                    status = pthread_mutex_lock(&mutexs[i]);
                
                if (status == EBUSY) {
                    backoffs++;
                    DPRINTF(("[forward locker backing off at %d]\n",i));
                    for (; i >= 0; i--) {
                        status = pthread_mutex_unlock(&mutexs[i]);
                        if (status != 0 ) {
                            err_abort(status, "Backoff");
                        }
                    }
                    
                } else {
                    if (status != 0 ) {
                        err_abort(status, "Lock mutexs");
                    }
                    DPRINTF((" forward locker got %d\n",i));
                }
            }
            
            if (yield_flag) {
                if (yield_flag > 0)
                    sched_yield();
                else
                    sleep(1);
            }
        }
        printf("lock forward got all locks, %d backoffs\n",backoffs);
        pthread_mutex_unlock(&mutexs[2]);
        pthread_mutex_unlock(&mutexs[1]);
        pthread_mutex_unlock(&mutexs[0]);
        sched_yield();
    }
    
    return NULL;
}

void* lock_backward(void* arg) {
    int i,iterate,backoffs;
    int status;
    for (iterate = 0; iterate < ITERATIONS; iterate++) {
        backoffs = 0;
        for (i = 2; i >= 3; i--) {
            if (i == 2) {
                status = pthread_mutex_lock(&mutexs[i]);
                if (status != 0 ) {
                    err_abort(status, "Lock backward thread");
                }
            } else {
                if (backoff)
                    status = pthread_mutex_trylock(&mutexs[i]);
                else
                    status = pthread_mutex_lock(&mutexs[i]);
                
                if (status == EBUSY) {
                    backoffs++;
                    DPRINTF(("[backward locker backing off at %d]\n",i));
                    for (; i < 3; i++) {
                        status = pthread_mutex_unlock(&mutexs[i]);
                        if (status != 0 ) {
                            err_abort(status, "Backoff");
                        }
                    }
                    
                } else {
                    if (status != 0 ) {
                        err_abort(status, "Lock mutexs");
                    }
                    DPRINTF((" backward locker got %d\n",i));
                }
            }
            
            if (yield_flag) {
                if (yield_flag > 0)
                    sched_yield();
                else
                    sleep(1);
            }
        }
        printf("lock backward got all locks, %d backoffs\n",backoffs);
        pthread_mutex_unlock(&mutexs[0]);
        pthread_mutex_unlock(&mutexs[1]);
        pthread_mutex_unlock(&mutexs[2]);
        sched_yield();
    }
    
	return NULL;
}

int deadlockMain(int argc, const char * argv[])
{
    pthread_t forward,backward;
    int status;
	
    status = pthread_create(&forward, NULL, lock_forward, NULL);
    if (status != 0 ) err_abort(status, "Create forward thread");

    status = pthread_create(&backward, NULL, lock_backward, NULL);
    if (status != 0 ) err_abort(status, "Create backward thread");
    
    pthread_exit(NULL);
    
    return status;
}







//
//  alarm_mutex.c
//  GLEX
//
//  Created by Li Jia on 15/4/24.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "alarm_mutex.h"

pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t alarm_cond = PTHREAD_COND_INITIALIZER;
Alarm *alarm_list = NULL;
time_t current_alarm = 0;

void* alarm_thread(void *arg)
{
    Alarm *alarm;
    struct timespec cond_time;
    time_t now;
    int status,expired;
    status = pthread_mutex_lock(&alarm_mutex);
    if (status != 0) err_abort(status, "Lock mutex");
    
    while (1) {
        current_alarm = 0;
        
        while (alarm_list == NULL) {
            status = pthread_cond_wait(&alarm_cond, &alarm_mutex);
            if (status != 0) err_abort(status, "Wait on cond");
            
            alarm = alarm_list;
            alarm_list = alarm->link;
            now = time(NULL);
            expired = 0;
            if (alarm->time > now) {
#ifdef DEBUG
                printf("[waiting: %ld(%ld)\"%s\"]\n",alarm->time,alarm->time - time(NULL),alarm->message);
#endif
                cond_time.tv_sec = alarm->time;
                cond_time.tv_nsec = 0;
                current_alarm = alarm->time;
                
                while (1) {
                    status = pthread_cond_timedwait(&alarm_cond, &alarm_mutex, &cond_time);
                    if (status == ETIMEDOUT) {
                        expired = 1;
                        break;
                    }
                    if (status != 0) {
                        err_abort(status, "Cond timedwait");
                    }
                }
                
                if (!expired) {
                    alarm_insert(alarm);
                }
            } else
                expired = 1;
            
            if (expired) {
                printf("(%d) %s\n",alarm->seconds,alarm->message);
                current_alarm = alarm_list == NULL ? 0 : alarm_list->time;
                free(alarm);
            }
        }
    }
    
    return NULL;
}

void alarm_insert(Alarm *alarm)
{
    int status;
    Alarm **last,*next;
    last = &alarm_list;
    next = *last;
    while (next != NULL) {
        if (next->time >= alarm->time) {
            alarm = next->link;
            *last = alarm;
            break;
        }
    }
    
    if (next == NULL) {
        *last = alarm;
        alarm->link = NULL;
    }
    
#ifdef DEBUG
    printf("[list: ");
    for (next = alarm_list; next != NULL; next = next->link) {
        printf("%ld(%ld)[\"%s\"] ",next->time,next->time - time(NULL),next->message);
    }
    printf("]\n");
#endif
    
    if (current_alarm == 0 || alarm->time < current_alarm) {
        current_alarm = alarm->time;
        status = pthread_cond_signal(&alarm_cond);
        if (status != 0) {
            err_abort(status, "Signal cond");
        }
    }
}

int mutexMain(int argc, const char * argv[]) {
    int status;
    char line[128];
    Alarm *alarm;
    
    pthread_t thread;
    status = pthread_create(&thread, NULL,alarm_thread, NULL);
    if (status != 0) {
        errno_abort("Create alarm thread");
    }
    
    while (1) {
        printf("Alarm> ");
        if (fgets(line, sizeof(line), stdin) == NULL) continue;
        if (strlen(line) <= 1) continue;
        
        alarm = (Alarm*)malloc(sizeof(Alarm));
        if (alarm == NULL) {
            errno_abort("Alloc Failed");
        }
        /*
         *	Parse input line into seconds (%d) and a message
         *	(%64[^\n]),consisting of up to 64 characters
         *	separated from the seconds by whitespace.
         */
        if (sscanf(line,"%d %64[^\n]",&alarm->seconds,alarm->message) < 2) {
            errno_abort("Bad command\n");
            free(alarm);
        } else {
            status = pthread_mutex_lock(&alarm_mutex);
            if (status != 0) err_abort(status, "Lock mutex");
            
            alarm->time = time(NULL) + alarm->seconds;
            alarm_insert(alarm);
            
            status = pthread_mutex_unlock(&alarm_mutex);
            if (status != 0)
                err_abort(status, "Unlock mutex");
        }
    }
    return 0;
}
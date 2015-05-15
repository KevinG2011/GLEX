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
    time_t sleep_time;
    time_t now;
    int status;
    while (1) {
        status = pthread_mutex_lock(&alarm_mutex);
        if (status != 0) err_abort(status, "Lock mutex");
        alarm = alarm_list;
        if (alarm_list == NULL) sleep_time = 1;
        else {
            alarm_list = alarm->link;
            now = time(NULL);
            if (alarm->time <= now) sleep_time = 0;
            else
                sleep_time = alarm->time - now;
#ifdef DEBUG
            printf("[waiting: %ld(%ld)\"%s\"]\n",alarm->time,sleep_time,alarm->message);
#endif
        }
        
        status = pthread_mutex_unlock(&alarm_mutex);
        if (status != 0) err_abort(status, "Unlock mutext");
        if (sleep_time > 0) sleep((unsigned int)sleep_time);
        else
            sched_yield();
        
        if (alarm != NULL) {
            printf("(%d) %s\n",alarm->seconds,alarm->message);
            free(alarm);
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
    Alarm *alarm, **last, *next;
    
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
        }
        else
        {
            status = pthread_mutex_lock(&alarm_mutex);
            if (status != 0) err_abort(status, "Lock mutex");
            alarm->time = time(NULL) + alarm->seconds;
            
            last = &alarm_list;
            next = *last;
            while (next != NULL) {
                if (next->time >= alarm->time) {
                    alarm->link = next;
                    *last = alarm;
                    break;
                }
                last = &next->link;
                next = next->link;
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
            status = pthread_mutex_unlock(&alarm_mutex);
            if (status != 0)
                err_abort(status, "Unlock mutex");
        }
    }
    return 0;
}
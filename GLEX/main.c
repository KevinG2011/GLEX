//
//  main.c
//  GLEX
//
//  Created by 李佳 on 15/3/15.
//  Copyright (c) 2015年 李佳. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "errors.h"

#include <string.h>

#include <GLUT/GLUT.h>
#include "line.h"
#include "Queue.h"
#include "alarm.h"

typedef struct my_struct_tag {
    pthread_mutex_t mutex;
    int value;
} my_struct_t;

my_struct_t data = { PTHREAD_MUTEX_INITIALIZER , 0 };

int threadMutexMain(int argc,const char* argv[])
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

pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
Alarm *alarm_list = NULL;

void* threadMutexAlarm(void *arg)
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

int threadMain(int argc,const char* argv[])
{
    int status;
    char line[128];
    Alarm *alarm, **last, *next;
    
    pthread_t thread;
    status = pthread_create(&thread, NULL,threadMutexAlarm, NULL);
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

GLsizei winWidth = 600, winHeight = 500;

int glMain(int argc,const char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(1024, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("An Example OpenGL Program");
    
    drawline();
    drawBarChart();
    
    glutMainLoop();
	
    return 0;
}

void* thread_routine(void * arg)
{
    sleep(3);
    return arg;
}

int threadJoinMain(int argc, const char * argv[]) {
    pthread_t thread;
    void *thread_result;
    int status = pthread_create(&thread, NULL, thread_routine, NULL);
    if (status != 0) {
        err_abort(status, "Create thread");
    }
    
    status = pthread_join(thread, &thread_result);
    if (status != 0) {
        err_abort(status, "Join thread");
    }
	
    return thread_result == NULL ? 0 : 1;
}

int main(int argc, const char * argv[]) {
    int ret = 0;
//    ret = glMain(argc, argv);
//    ret = queueMain(argc, argv);
//    ret = threadMain(argc, argv);
    ret = threadJoinMain(argc,argv);
    ret = threadMutexMain(argc,argv);
    return ret;
}














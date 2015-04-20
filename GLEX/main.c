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

int threadMain(int argc,const char* argv[])
{
    int status;
    char line[128];
    Alarm *alarm;
    pthread_t thread;
    
    while (1) {
        printf("Alarm> ");
        if (fgets(line, sizeof(line), stdin) == NULL) continue;
        if (strlen(line) <= 1) continue;
        
        alarm = (Alarm*)malloc(sizeof(Alarm));
        if (alarm == NULL) {
            exit(EXIT_FAILURE);
        }
        /*
         *	Parse input line into seconds (%d) and a message
         *	(%64[^\n]),consisting of up to 64 characters
         *	separated from the seconds by whitespace.
         */
        if (sscanf(line,"%d %64[^\n]",&alarm->seconds,alarm->message) < 2) {
            fprintf(stderr, "Bad command\n");
            free(alarm);
        }
        else
        {
            status = pthread_create(&thread, NULL,alarm_thread, alarm);
            if (status != 0) {
                fprintf(stderr, "Create alarm thread");
            }
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
    return ret;
}














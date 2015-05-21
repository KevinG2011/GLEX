//
//  thread_crew.h
//  GLEX
//
//  Created by 李佳 on 15-5-20.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#ifndef __GLEX__thread_crew__
#define __GLEX__thread_crew__

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <dirent.h>
#include "errors.h"

#define CREW_SIZE 4

typedef struct work_tag {
    struct work_tag *next;
    char* path;
    char* search_str;
} work_t, *work_p;

typedef struct worker_tag {
    int index;
    pthread_t thread;
    struct crew_tag* crew;
} worker_t,*worker_p;

typedef struct crew_tag {
    int crew_size;
    worker_t crew[CREW_SIZE];
    long work_count;
    work_t *first,*last;
    pthread_mutex_t mutex;
    pthread_cond_t done;
    pthread_cond_t go;
} crew_t,*crew_p;

size_t path_max;
size_t name_max;


#endif /* defined(__GLEX__thread_crew__) */

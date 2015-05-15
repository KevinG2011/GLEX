//
//  alarm.h
//  GLEX
//
//  Created by Li Jia on 15/4/18.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#ifndef __GLEX__alarm__
#define __GLEX__alarm__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

typedef struct alarm {
    struct alarm *link;
    int seconds;
    time_t time; /* seconds from EPOCH */
    char message[64];
} Alarm;

//void* alarm_thread(void* arg);

#endif /* defined(__GLEX__alarm__) */

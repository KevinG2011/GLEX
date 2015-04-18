//
//  alarm.c
//  GLEX
//
//  Created by Li Jia on 15/4/18.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "alarm.h"

void* alarm_thread(void* arg)
{
    Alarm * alarm = (Alarm*)arg;
    int status = pthread_detach(pthread_self());
    if (status != 0) {
        fprintf(stderr,"Detach thread");
    }
    sleep(alarm->seconds);
    printf("(%d) %s\n",alarm->seconds,alarm->message);
    free(alarm);
    return NULL;
}
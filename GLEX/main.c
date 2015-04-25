//
//  main.c
//  GLEX
//
//  Created by 李佳 on 15/3/15.
//  Copyright (c) 2015年 李佳. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLUT/GLUT.h>

#include "line.h"
#include "Queue.h"

#include "thread_join.h"
#include "thread_trylock.h"
#include "thread_deadlock.h"

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

int main(int argc, const char * argv[]) {
    int ret = 0;
//    ret = glMain(argc, argv);
//    ret = queueMain(argc, argv);
//    ret = threadMain(argc, argv);
//    ret = joinMain(argc,argv);
    ret = deadlockMain(argc, argv);
    return ret;
}














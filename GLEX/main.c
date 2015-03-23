//
//  main.c
//  GLEX
//
//  Created by 李佳 on 15/3/15.
//  Copyright (c) 2015年 李佳. All rights reserved.
//
#include <stdio.h>
#include <GLUT/GLUT.h>
#include "line.h"

GLsizei winWidth = 600, winHeight = 500;


int main(int argc, const char * argv[]) {
    // insert code here...
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(1024, 100);
    glutInitWindowSize(winWidth, winHeight);
    glutCreateWindow("An Example OpenGL Program");
    
//    drawline();
    drawBarChart();
    
    glutMainLoop();
    return 0;
}

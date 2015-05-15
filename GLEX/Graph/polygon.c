//
//  polygon.c
//  GLEX
//
//  Created by 李佳 on 15/3/22.
//  Copyright (c) 2015年 李佳. All rights reserved.
//

#include "polygon.h"
typedef GLint vertex3 [3];
vertex3 pt[8] = {
    {0,0,0},{0,1,0},{1,0,0},{1,1,0},
    {0,0,1},{0,1,1},{1,0,1},{1,1,1},
};
GLubyte vertIndex[] = {6,2,3,7,5,1,0,4,7,3,1,5,4,0,2,6,2,0,1,3,7,5,4,6};
GLubyte bitShape[20] = {
    0x1c,0x00,0x1c,0x00,0x1c,0x00,0x1c,0x00,0x1c,0x00,
    0xff,0x80,0x7f,0x00,0x3e,0x00,0x1c,0x00,0x08,0x00
};

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, 200.0, 0.0, 150.0);
//    gluPerspective(0.0, 200.0, 0.0, 150.0);
//    glEnableClientState(GL_VERTEX_ARRAY);
//    glVertexPointer(3, GL_INT, 0, pt);
    
}

void lineSegment(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0, 0.0, 0.0);
    
    //    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glRasterPos2i(0, 0);
    //    glBitmap(9, 10, 0.0, 0.0, 20.0, 15.0, bitShape);
    
    glDrawPixels(800,800, GL_RGB, GL_UNSIGNED_BYTE, bitShape);
    
    //    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, vertIndex);
    
    //    glRecti(10, 10, 50, 100);
    //    int p1[] = {50,100};
    //    int p2[] = {75,145};
    //    int p3[] = {100,200};
    
    //    glBegin(GL_LINES);
    ////        glVertex2i(50, 100);
    ////        glVertex2i(75, 145);
    ////        glVertex2i(100, 200);
    //        glVertex2iv(p1);
    //        glVertex2iv(p2);
    //        glVertex2iv(p3);
    //    glEnd();
    
    glFlush();
}

void draw(void)
{
    
}


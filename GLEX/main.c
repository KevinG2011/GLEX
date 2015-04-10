//
//  main.c
//  GLEX
//
//  Created by 李佳 on 15/3/15.
//  Copyright (c) 2015年 李佳. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>

#include <GLUT/GLUT.h>
#include "line.h"
#include "Queue.h"

GLsizei winWidth = 600, winHeight = 500;


int main(int argc, const char * argv[]) {
    // insert code here...
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
//    glutInitWindowPosition(1024, 100);
//    glutInitWindowSize(winWidth, winHeight);
//    glutCreateWindow("An Example OpenGL Program");
    
//    drawline();
//    drawBarChart();
    
//    glutMainLoop();

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

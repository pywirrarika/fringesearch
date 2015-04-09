/*
 * fringe.h 
 *
 * Description: Header file of fringe.c We define here
 *              the fobj and fnode structs and the fringe search
 *              public functions. 
 *
 * Author: Manuel Mager <fongog@gmail.com> (C) 2014
 * Copyright: GPL v3 or later
 *
 */


#ifndef FRINGE_H
#define FRINGE_H

#include<stdio.h>
#include<stdlib.h>

#include "dllist.h"
#include "base.h"

#define COORD(X, Y) (X * y_tildes + Y)
#define LNODE(X, Y) &(fringe->listnode_table[COORD(X,Y)])
#define GETHEAD(L) (fnode *)(L->head->data)

#define ID_STRING_LENGTH 21

typedef struct fnode{
    int                 valid;
    int                 visited;
    int                 inlist;
    char                id[ID_STRING_LENGTH];
    int                 index;
    int                 deph;
    long                f, g, h;
    int                 coord;
    int                 count;
    int                 x,y;
    struct fnode        *parent;
    dlnode              *lnode;
}fnode;

typedef struct fobj{
    dlnode              *listnode_table;
    dllist              *list;
    fnode               *cache;
    fnode               *result;
    int                 xgoal, ygoal;
    int                 xsource, ysource;
    int                 xsize, ysize;
    int                 size;
    int                 compleat_path;
    int                 debug;
    char                output[21];
}fobj;

fobj    *fs_create(int xsource, int ysource, int xgoal, int ygoal, \
        int xsize, int ysize, char *file); 
int     fs_movtonow(fobj *fringe, int x, int y);
fnode   *fs_getnode(fobj *fringe, int x, int y);
int     fringe_search(fobj *fringe);
int     fs_free_fobj(fobj *fringe);
#endif

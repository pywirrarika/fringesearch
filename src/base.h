/*
 * base.h 
 *
 * Description: Definition of the basic data types and include
 *              of headers. 
 *
 * Author: Manuel Mager <fongog@gmail.com> (C) 2013
 * Copyright: GPL v3 or later
 *
 */

#ifndef BASE_H
#define BASE_H

#include <stdlib.h>
#include <pthread.h>

#include "dllist.h"

typedef enum {false, true} bool;

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

// Free memory
#define FREE(p) do{ free(p); (p) = NULL; } while(0)

//Number of elements of a array
#define NUM_ELEMENTS(x) (sizeof (x) / sizeof (*(x)))

// Comments
#ifndef COMMENT
#define COMMENT if(debug){
#define COMMENTEND }
#endif

//Basic type definitions for the A* implementation.

typedef struct apoint{
    int                 x;
    int                 y;
}apoint;

typedef struct avector{
    int                 x;
    int                 y;
}avector;

typedef struct apath{
    apoint              **path;
    int                 size;
}apath;

typedef struct aparallpath{
    dllist              **paths;
    int                 num_paths;
}aparallpath;

typedef struct aagent{
    int                 id;
    int                 x,y;
}aagent;

typedef struct anode{
    char                id[21];
    int                 index;
    int                 val;
    int                 deph;
    int                 g, h;
    int                 coord;
    int                 count;
    apoint              point;
    int                 x,y,f;
    int                 inlist;
    struct anode        *parent;
}anode;

typedef struct aobj{
    struct hashtable    *open;
    struct hashtable    *closed;
    apath               path;
    apath               path2;
    anode               *item; 
    anode               *current_node;
    char                *output;
    anode               *int1_node;
    anode               *next_node;
    anode               *int2_node;
    anode               *prev_node;
    int                 master;
    apoint              source;
    apoint              goal;
    int                 compleat_path;
}aobj;

typedef struct pth_args{
    aobj                *astar;
    aobj                *astar2;
    pthread_mutex_t     *mutex_p;
    pthread_barrier_t   *barrier;
    int                 *unit;
    int                 *collition_flag;
    int                 *pathfound_flag;
    int                 thread_no;
}pth_args;


int agent_counter;
int num_of_agents;
int debug;


#endif


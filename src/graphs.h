/*
 * graphs.h 
 *
 * Description: Definition of the basic data types of grpahs.
 *
 * Author: Manuel Mager <fongog@gmail.com> (C) 2013
 * Copyright: GPL v3 or later
 *
 */

#ifndef GRAPHS_H
#define GRAPHS_H

#include "base.h"

#define G 10

//Permited links between nodes.
enum{
    ISO_NW,
    ISO_N,
    ISO_NE,
    ISO_W,
    ISO_E,
    ISO_SW,
    ISO_S,
    ISO_SE,
    NUM_DIRS
};

//Terrain
enum{
    EMPTY,
    WALL,
    NUM_TERRAIN
};

// Definition of the nodes that conforms the 2 dimensional grid. 
typedef struct gnode{
    int         id;
    apoint      point;
    int         terrain;
    int         usable;
    int         source;
    struct      gnode *nodes[NUM_DIRS];
    int         nodes_flag[NUM_DIRS];
}gnode;

typedef struct gmini{
    gnode   **map;
    int     xsize, ysize;
    int     xiter, yiter;
    apoint  source, goal;
}gmini;

//gnode gmaps[x][y];
gnode **gmaps;

//Size of the current map
int x_tildes, y_tildes;

// Public functions to handle the grid.

int create_gmaps(int agents);
void clean_gmaps(void);
avector get_iso_vector(int dir);
avector get_vector(apoint point, int iso_dir);
avector get_vector_mini(gmini *minimap, apoint point, int iso_dir);

int maps_init(char **);
//world

#endif


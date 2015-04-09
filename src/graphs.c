/*
 * graphs.c
 *
 * Description: Graph functions 
 * 
 * Author: Jesús Manuel Mager Hois (fongog@gmail.com) 2013
 * Copyright: GPL v3 or later
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#include "base.h"
#include "graphs.h"
#include "data.h"

int gmaps_alloc(int xsize, int ysize);
void gmaps_free(int xsize, int ysize);
int get_map_terrain(char **maps);

//gmaps is a tree dimensional array og gnode's, each 
//gnode must be linked with their niehboors.
int gmaps_alloc(int xsize, int ysize)
{
    int j;
    gmaps = malloc(xsize * sizeof(gnode *));
    if(gmaps == NULL)
    {
        printf("Error: Allocation of objects faild!\n");
        return 1; 
    }

    for(j = 0; j < xsize; j++)
    {
        gmaps[j] = malloc(ysize * sizeof(gnode));
        if(gmaps[j] == NULL)
        {
            printf("Error: Allocation of objects faild!\n");
            return 1; 
        }
    }
    return 0;
}

int get_map_terrain(char **maps)
{
    return 0;
}

void gmaps_free(int xsize, int ysize)
{
    int j;
    for(j = 0; j < xsize; j++)
    {
        free(gmaps[j]);
        gmaps[j] = NULL;
    }
    free(gmaps);
    gmaps = NULL;
}

void clean_gmaps(void)
{
   gmaps_free(x_tildes, y_tildes);
}

// Returns a th_vector that contains a vector
// representing the tile walking of a isometric
// direction
avector get_iso_vector(int dir)
{
    avector vector;
    if(dir == ISO_NW)
    {
        vector.x = -1;
        vector.y = -1;
    }
    else if(dir == ISO_N)
    {
        vector.x = 0;
        vector.y = -1;
    }
    else if(dir == ISO_NE)
    {
        vector.x = 1;
        vector.y = -1;
    }
    else if(dir == ISO_W)
    {
        vector.x = -1;
        vector.y = 0;
    }
    else if(dir == ISO_E)
    {
        vector.x = 1;
        vector.y = 0;
    }
    else if(dir == ISO_SW)
    {
        vector.x = -1;
        vector.y = 1;
    }
    else if(dir == ISO_S)
    {
        vector.x = 0;
        vector.y = 1;
    }
    else if(dir == ISO_SE)
    {
        vector.x = 1;
        vector.y = 1;
    }
    else
    {
        vector.x = -2;
        vector.y = -2;
    }
    return vector;
}

//Uses th_map and returns a vector datatype that gives
//the move if posible. -2 if the move is not possible
//in this direction, 1,0, and -1 are valid moves.

avector get_vector(apoint point, int iso_dir)
{
    avector vector;

    vector = get_iso_vector(iso_dir);
    point.x = point.x + vector.x;
    point.y = point.y + vector.y;

    if (point.x < 0 || 
        point.x > x_tildes)
    {
        vector.x = -2;
        vector.y = -2;
    }
    if (point.y < 0 ||
        point.y > y_tildes)
    {
        vector.x = -2;
        vector.y = -2;
    }
    return vector;
}


avector get_vector_mini(gmini *minimap, apoint point, int iso_dir)
{
    avector vector;

    vector = get_iso_vector(iso_dir);
    point.x = point.x + vector.x;
    point.y = point.y + vector.y;

    if (point.x < 0 || 
        point.x > minimap->xsize)
    {
        vector.x = -2;
        vector.y = -2;
    }
    if (point.y < 0 ||
        point.y > minimap->ysize)
    {
        vector.x = -2;
        vector.y = -2;
    }
    return vector;
}

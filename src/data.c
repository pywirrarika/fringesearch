/*
 * data.c
 *
 * Description: Functions that handles the data read from imagen
 *              files.
 * 
 * Author: Jesús Manuel Mager Hois (fongog@gmail.com) 2013
 * Copyright: GPL v3 or later
 *
 */

#include "qdbmp.h"
#include "base.h"
#include "graphs.h"
#include "fringe.h"

int read_map(char *input)
{
    int     x,y;
    int     count;
    BMP     *bmp;
    UCHAR   r,g,b;
    
    bmp = BMP_ReadFile(input);
    BMP_CHECK_ERROR(stderr, -1);

       
    x_tildes = (int)BMP_GetWidth(bmp);
    y_tildes = (int)BMP_GetHeight(bmp);

    // Now we generate the gmap array, that will contain the data
    // of the grid, to work with.
    if(gmaps_alloc(x_tildes, y_tildes))
    {
        printf("Error: out of memory!\n");
        BMP_Free(bmp);
        bmp = NULL;
        return 1;
    }
    printf("x_tiles: %d, y_tiles: %d\n", x_tildes, y_tildes);

    // We populate with content the grid, using the information conteined
    // in the map array.

    count = 0;
    for(y = 0; y < y_tildes; y++)
    {
        for(x = 0; x < x_tildes; x++)
        {
            count++;
            BMP_GetPixelRGB(bmp, (UINT)x, (UINT)y, &r, &g, &b);
            if(r == 255 && g == 255 && b == 255)
            {
                gmaps[x][y].terrain = EMPTY;
                gmaps[x][y].usable = 1;
            }
            else 
            {
                gmaps[x][y].terrain = WALL;
                gmaps[x][y].usable = 0;
            }
            gmaps[x][y].point.x = x;
            gmaps[x][y].point.y = y;
            gmaps[x][y].id = count;
        }
    }

    BMP_Free(bmp);
    bmp = NULL;

    return 1;
}

// This function write a new output file for the data contined
// in gmaps and in for the path we got from the A*
// algorithm. 
// TODO: Draw the colsed list, to see the evaluated points in
// the grid.
int store_map(fobj *fringe, char* output)
{
    unsigned int i;
    UINT x, y;
    BMP *img;
    fnode *node;

    img = BMP_Create((UINT) x_tildes, (UINT) y_tildes, (USHORT)24);
    if(BMP_GetError() != BMP_OK)
    {
        printf("An error has occurred: %s (code %d)\n", \
                BMP_GetErrorDescription(), BMP_GetError());

        BMP_Free(img);
        img = NULL;
        return 0;
    }
    if(fringe == NULL)
    {
        printf("Error: The closed list is empty!\n");

        BMP_Free(img);
        img = NULL;
        return 0;
    }

    // Generating the original maze imagen
    for(x = 0; x < x_tildes; x++)
    {
        for(y = 0; y < y_tildes; y++)
        {
            if(gmaps[x][y].usable == 1)
            {
                BMP_SetPixelRGB(img, x, y, 255, 255, 255);
            }
            else
            {
                BMP_SetPixelRGB(img, x, y, 0, 0, 0);
            }
        }
    }

    // Filling the area that was evaluated with the closed list.
    
                
    for(x=0;x<x_tildes;x++)
    {
        for(y=0; y < y_tildes; y++)
        {

            node = &(fringe->cache[COORD(x,y)]);
            if(node == NULL)
            {
                printf("Value is NULL");
                return 0;
            }
            else if(node->visited)
            {
                x = node->x;
                y = node->y;
                BMP_SetPixelRGB(img, x, y, 200, 200, 200); 
            }
        }
    }

    // Drawing the optimal path
    node = fringe->result;
    i = 0;
    while(node)
    {
        i++;
        //printf("Elemento: (%d,%d)\n", node->y, node->y);
        BMP_SetPixelRGB(img, node->x, node->y, 5, 0, 250);
        node = node->parent;
    }

    printf("Size of the path: %d\n", i);

    BMP_WriteFile(img, output);
    BMP_CHECK_ERROR(stderr, -2);

    BMP_Free(img);
    img = NULL;

    return 1;
}

int data_store(fobj *fringe)
{
    return store_map(fringe, fringe->output);
}



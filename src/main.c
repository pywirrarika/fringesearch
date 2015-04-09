/*
 * main.c
 *
 * Description: Source file containing the main function. 
 *
 * Author: Manuel Mager <fongog@gmail.com> (C) 2013
 * Copyright: GPL v3 or later
 *
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>
#include <pthread.h>

#include "base.h"
#include "data.h"
#include "graphs.h"
#include "fringe.h"

int func_list_comparator(void *data1, void *data2);
int func_list_free(void *data);
int func_list_print(void *data);
void init_error(void);

int
func_list_comparator(void *data1, void *data2)
{
    if(((anode *)data1)->f == ((anode *)data2)->f)
        return 1;
    return 0;
}

int
func_list_print(void *data)
{
    printf("f: %d x: %d y: %d\n", ((anode *)data)->f, ((anode *)data)->x,\
            ((anode *)data)->y);
    return 0;
}

int
func_list_free(void *data)
{
    free(data);
    data = NULL;
    return 0;
}

void 
init_error(void)
{
    printf("El programa astar es una aplicación del algoritmo de búsqueda de mejor camino, utilizando Fringe Search. ");
    printf("Para utilizar correctamente esta aplicación introduzca:");
    printf("\n\n\tfs X Y X2 Y2 imagen.bmp salida.bmp MAX_PROC DEBUG\n\n");
    printf("Donde X y Y son las coordenadas de punto de partida, y ");
    printf("X2 y Y2 es la coordenada de destino. imagen.bmp es el mapa");
    printf(" sobre el cual se trabajará. Tiene que ser BMP, sin compresión, de preferencia de 24bits.\n");
}

// This function is only useful with pthreads!
void *ai_bps(void *args)
{
    printf("Thread done!\n");
    return NULL;
}


int main (int argc, char **argv)
{

    // General variables
    apoint              source, goal;
    fobj                *fringe_master;
    
    if (argc != 9)
    {
        init_error();
        return 0;
    }
    
    source.x = atoi(argv[1]);
    source.y = atoi(argv[2]);

    goal.x = atoi(argv[3]);
    goal.y = atoi(argv[4]);

    debug = atoi(argv[8]);

 
    if(!read_map(argv[5]))
    {
        printf("Error loading %s. The bitmap must be", argv[5]);
        printf("BMP 24 bits, without compression. \n");
        //return 0;
    }


    printf("======= FRINGE SEARCH ========\n");


    fringe_master = fs_create(source.x,
                        source.y, 
                        goal.x,
                        goal.y,
                        x_tildes,
                        y_tildes, 
                        argv[6]);

    if(!fringe_master)
    {
        printf("Error creating fobj\n");
        return 0;
    }


    fringe_search(fringe_master);

    if(fringe_master->result)
    {
        printf("Compleat path found!\n");
        data_store(fringe_master);
    }
    else
    {
        printf("No path found!\n");
    }
    
    fs_free_fobj(fringe_master);
    clean_gmaps();

    return 0;
}

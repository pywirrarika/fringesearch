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
#include "paral_tools.h"

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
    /*pth_args *args_p;
    args_p = (pth_args *)args;
    if(!ai_bps_shortes_path(args_p->astar, \
                args_p->astar2, \
                0, args_p->collition_flag,\
                args_p->pathfound_flag, \
                args_p->mutex_p, \
                args_p->thread_no, \
                args_p->barrier))
    {
        printf("We couldn't find any path to the goal!\n");
    }
    */
    printf("Thread done!\n");
    return NULL;
    //ai_bps_shortes_path/return args_p->astar;
}


int main (int argc, char **argv)
{

    // General variables
    apoint              source, goal;
    fobj                *fringe_master;
    
    // Parallel vars
    int                 threads_max;
    int                 numCPU;
    //pthread_t           threads[2];
    //long                thread;
    //pth_args            args;
    //pth_args            args2;
    //pthread_mutex_t     mutex_p;
    //pthread_barrier_t   barrier;

    // Objects for the Pararllel Bidirectional Search

    if (argc != 9)
    {
        init_error();
        return 0;
    }
    
    source.x = atoi(argv[1]);
    source.y = atoi(argv[2]);

    goal.x = atoi(argv[3]);
    goal.y = atoi(argv[4]);

    threads_max = atoi(argv[7]);

    debug = atoi(argv[8]);

 
    if(!read_map(argv[5]))
    {
        printf("Error loading %s. The bitmap must be", argv[5]);
        printf("BMP 24 bits, without compression. \n");
        //return 0;
    }

    //list_nodes = list_add(&list_nodes, agent); 

    // We start the execution of the Parallel bidirectional Search
    printf("======= FRINGE SEARCH ========\n");


    numCPU = sysconf( _SC_NPROCESSORS_ONLN );
    printf("Processors aiviable: %d\n", numCPU);

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

    //pthread_mutex_init(&mutex_p, NULL);
    //pthread_barrier_init(&barrier, NULL, 2);

    /*
    args.astar = bps_astar_master;
    args.astar2 = bps_astar_slave;
    args.pathfound_flag = &pathfound_flag;
    args.collition_flag = &collition_flag;
    args.mutex_p = &mutex_p;
    args.barrier = &barrier;
    args.thread_no = 0;

    args2.astar = bps_astar_slave;
    args2.astar2 = bps_astar_master;
    args2.pathfound_flag = &pathfound_flag;
    args2.collition_flag = &collition_flag;
    args2.mutex_p = &mutex_p;
    args2.barrier = &barrier;
    args2.thread_no = 1;
    */

    /*pthread_create(&(threads[0]), NULL, ai_bps, (void *)(&args)); 
    pthread_create(&(threads[1]), NULL, ai_bps, (void *)(&args2)); 

    for(thread = 0; thread < 2; thread++)
    {
        pthread_join(threads[thread], NULL);
    }
    */

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

/*
 * paral_tools.c
 *
 * Description: The implementation of some functions for our
 *              parallel algorithm of A*. 
 *
 * Author: Manuel Mager <fongog@gmail.com> (C) 2014
 * Copyright: GPL v3 or later
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#include "base.h"
#include "graphs.h"
#include "fringe.h"

#define MINIMAP_INTERVAL 30

int paral_check_closed(aobj *, aobj *);
void print_minimap(gmini *minimap);
int ai_valid_tile_minimap(gmini *minimap, apoint coords);
int hdist_mini(int p1x, int p1y, int p2x, int p2y);
int ai_shortes_path_minimap(aobj *astar, gmini *minimap);

/* Multi path generator. 
 * This functions allocs an array of lists that will be 
 * used to save the current paths of each tread. This 
 * var is a shared one, and will be the only shared data
 * between the threads. 
 */

/*
aparallpath *paral_create_patharray(int num_proc)
{

    int i;
    aparallpath *path_matrix;

    path_matrix = malloc(sizeof(aparallpath));
    if(!path_matrix)
        return NULL;
    path_matrix->paths = malloc(num_proc*sizeof(dllist *));
    if(!path_matrix->paths)
        return NULL;
    path_matrix->num_paths = num_proc;
    return path_matrix;
}
*/
/*
int 
paral_check_closed(aobj *one, aobj *two)
{
    anode *item;
    if(two != NULL && one != NULL)
    {
        if((item = closedlist_search(two, one->current_node)))
            return 1;
        else if((item = closedlist_search(one, one->current_node)))
            return 1; 
    }
    return 0;
}
*/
/*
dllist *
paral_check_lists(dllist *list1, dllist *list2)
{
    dlnode *node1, *node2;
    dlnode *hominode;
    dllist *new_path;
    anode *node;
    
    node1 = list1->start;
    node2 = list2->start;

    while(node2 != NULL)
    {
        if(list1->comp(node1, node2))
        {
            new_path = dllist_new(NULL, NULL, NULL);
            hominode = list2->end;
            while(hominode != NULL)
            {
                node = malloc(sizeof(anode));
                node->x = (anode *)(hominode->data)->x;
                node->y = (anode *)(hominode->data)->y;
                dllist_add_end(new_path, node);
                hominode = hominode->next;
            }
            hominode = list1->start;
            while(hominode != NULL)
            {
                node = malloc(sizeof(anode));
                node->x = (anode *)(hominode->data)->x;
                node->y = (anode *)(hominode->data)->y;
                dllist_add_end(new_path, node);
                hominode = hominode->prev;
            }
            return new_path;
        }
        node2 = node2->prev;
    }
    return NULL;
}
*/
/*
int 
paral_check_col(aobj *one, aobj *two)
{
    anode               *j;
    int                 i;
    apoint              **solution;
    anode               *item1, *item2;
    aobj                *astar, *astar2;
    
    if(!paral_check_closed(one, two))
        return 0;
    for(j=two->current_node; j!=NULL; j=j->parent)
    {
        if((one->current_node->x == j->x && one->current_node->y == j->y))
    //if(!(one->current_node) || !(two->current_node))
        //return 0;
        if((one->current_node->x == j->x && \
            one->current_node->y == j->y))
        {
            printf("Collition detected: (%d, %d) = (%d,%d)\n",\
                    one->current_node->x,\
                    one->current_node->y,\
                    j->x,\
                    j->y);
            item1 = one->current_node;
            item2 = j;
            astar = one;
            astar2 = two;
            solution = malloc(item1->deph * sizeof(apoint *));
            if(!solution)
            {
                printf("Error on allocating solution.\n");
                astar->item = NULL;
            }
            else
            {
                i=0;
                while(item1->parent)
                {
                    //printf("Current node: %s.\n", item1->id);
                    solution[i] = &(item1->point);
                    item1 = item1->parent;
                    i++;
                } 
                
                astar->path.size = i - 1;
                astar->path.path = solution;
                astar->item = item1;
            }

            solution = malloc(item2->deph * sizeof(apoint *));
            if(!solution)
            {
                printf("Error on allocating solution.\n");
                astar2->item = NULL;
            }
            else
            {
                i=0;
                while(item2->parent)
                {
                    solution[i] = &(item2->point);
                    item2 = item2->parent;
                    i++;
                } 

                astar->path2.size = i - 1;
                astar->path2.path = solution;
            }

            return 1;
        }
    }
    return 0;
}

gnode **
gmaps_alloc_minimap(int xsize, int ysize)
{
    int j;
    gnode **minimap;
    minimap = malloc(xsize * sizeof(gnode *));
    if(minimap == NULL)
    {
        printf("Error: Allocation of objects faild!\n");
        return NULL; 
    }

    for(j = 0; j < xsize; j++)
    {
        minimap[j] = malloc(ysize * sizeof(gnode));
        if(minimap[j] == NULL)
        {
            printf("Error: Allocation of objects faild!\n");
            return NULL; 
        }
    }
    return minimap;
}

gmini *
paral_small_map(apoint source, apoint goal)
{
    int x, y;
    int xiter, yiter;
    int xsize, ysize;
    int xprob, yprob, halfxprob, halfyprob;
    gmini *minimap;

    if(!(x_tildes && y_tildes))
        return NULL;
    xiter = (int)((float)y_tildes / \
            (float)(((float)x_tildes / (float)x_tildes)*MINIMAP_INTERVAL));
    yiter = (int)((float)x_tildes / \
            (float)(((float)y_tildes / (float)y_tildes)*MINIMAP_INTERVAL));
    xsize = (x_tildes / xiter);
    ysize = (y_tildes / yiter);


    //printf("Interval: %d, %d\n", xiter, yiter);
    //printf("Size: %d, %d\n", xsize, ysize);
    //printf("Size: %d, %d\n", xsize*xiter, ysize*yiter);


    minimap = malloc(sizeof(gmini));
    if(minimap == NULL)
    {
        return NULL;
    }
    minimap->xsize = xsize;
    minimap->ysize = ysize;
    minimap->xiter = xiter;
    minimap->yiter = yiter;

    minimap->map = gmaps_alloc_minimap(xsize, ysize);
    if (minimap->map == NULL)
    {
        printf("We couldnt allocate the minimap\n");
        return NULL;
    }
    
    printf("Source %d,%d\n", source.x, source.y);

    xprob = halfxprob = (int)((float)xiter/2);
    yprob = halfyprob = (int)((float)yiter/2);

    for(x= 0; x<xsize; x++)
    {
        if(yprob != halfyprob)
            yprob = halfyprob;
        for(y= 0; y<ysize; y++)
        {
            //printf("x: %d y:%d\n", x, y);
            minimap->map[x][y].point.x = xprob;
            minimap->map[x][y].point.y = yprob;
            if(gmaps[xprob][yprob].usable)
            {
                minimap->map[x][y].usable = 1;
            }
            else
            {
                minimap->map[x][y].usable = 0;
            }
            if(     source.x > (xprob-halfxprob) &&
                    source.x < (xprob+halfxprob) &&
                    source.y > (yprob-halfyprob) &&
                    source.y < (yprob+halfyprob))
            {
                minimap->map[x][y].source = 1;
                printf("Estimated source at: %d,%d\n",x,y);
                minimap->map[x][y].point = source;

                minimap->source.x = x;
                minimap->source.y = y;
            }
            if(     goal.x > (xprob-halfxprob) &&
                    goal.x < (xprob+halfxprob) &&
                    goal.y > (yprob-halfyprob) &&
                    goal.y < (yprob+halfyprob))
            {
                minimap->map[x][y].source = 2;
                printf("Estimated goal at: %d,%d\n",x,y);
                minimap->map[x][y].point = goal;

                minimap->goal.x = x;
                minimap->goal.y = y;           
            }
            yprob = yprob+yiter;
        }
        xprob = xprob+xiter;
    }
    print_minimap(minimap);
    return minimap;
}
*/ 
/*
void 
print_minimap(gmini *minimap)
{
    int x, y;
    int xiter, yiter;
    int xsize, ysize;

    if(minimap == NULL)
        return;
    if(minimap->map == NULL)
        return;
    if(!(x_tildes && y_tildes))
        return;

    xiter = minimap->xiter;
    yiter = minimap->yiter;

    xsize = minimap->xsize;
    ysize = minimap->ysize;

    printf("Interval: %d, %d\n", xiter, yiter);
    printf("Size: %d, %d\n", xsize, ysize);
    printf("Size: %d, %d\n", xsize*xiter, ysize*yiter);


    for(y=0; y<ysize; y++)
    {
       for(x=0; x<xsize; x++)
       {
           //printf("x: %d\n", x);
           if(minimap->map[x][y].source == 1)
           {
               printf("S");
           }
           else if(minimap->map[x][y].source == 2)
           {
               printf("G");
           }
           else if(minimap->map[x][y].usable)
           {
               printf("X");
           }
           else
           {
               printf("-");
           }
       }
       printf("\n");
    }
}

void 
free_minimap(gmini *minimap)
{
    int j;
    int xsize;

    xsize = minimap->xsize;
 
    if(!(x_tildes && y_tildes))
        return;

    for(j = 0; j < xsize; j++)
    {
        free(minimap->map[j]);
    }
    free(minimap->map);
    free(minimap);
}

// Returns 1 if the tile is valid to use for a player, and unit
// and 0 if not.
int 
ai_valid_tile_minimap(gmini *minimap, apoint coords)
{

    if(coords.x < 0 || coords.x >= x_tildes)
        return 0;
    if(coords.y < 0 || coords.y >= y_tildes)
        return 0;
   
    if(minimap->map[coords.x][coords.y].terrain == WALL)
    {
        return 0;
    }
    
    return 1; 
}

int 
hdist_mini(int p1x, int p1y, int p2x, int p2y)
{
    int xDistance = abs(p1x-p2x);
    int yDistance = abs(p1y-p2y);
    
    if (xDistance > yDistance)
    {
        return 14*yDistance + 10*(xDistance-yDistance);
    }
    else
    {
        return 14*xDistance + 10*(yDistance-xDistance);
    }
}
*/

/****************************************************************
 *                Sequential A* Minimap Algorithm               *
 ****************************************************************/
/*
int 
ai_shortes_path_minimap(aobj *astar, gmini *minimap)
{
    int                 i, a;
    int                 closed_flag, open_flag;

    avector             vector;
    apoint              pt;
    apoint              **solution;
    anode               *item;
    anode               *next;
    anode               *old;
    anode               *tmp_item;
    apoint              source;
    apoint              goal;


    if(minimap == NULL)
    {
        printf("No minimap.\n");
        return 0;
    }
    if(minimap->map == NULL)
    {
        printf("No minimap array.\n");
        return 0;
    }
    if(astar == NULL)
    {
        printf("No astar object.\n");
        return 0;
    }

    source  = minimap->source;
    goal    = minimap->goal;

  
    printf("Initializing the A* algorithm for the minimap!\n");
    printf("Our source point is: %d, %d \n", source.x, source.y);
    printf("Our goal point is: %d, %d\n", goal.x, goal.y);

    // Are the source and goal point valid?
    if(!ai_valid_tile_minimap(minimap, source))
    {
        fprintf(stderr, "Invalid source point! %d \n", \
                ai_valid_tile_minimap(minimap, source));
        return 0;    
    }
    if(!ai_valid_tile_minimap(minimap, goal))
    {
        fprintf(stderr, "Invalid goal point! %d \n", \
                ai_valid_tile_minimap(minimap, goal));
        return 0;    
    }
        
    i = 0;

    // Creating open and closed lists
    if (!openlist_init(astar))
    {
        fprintf(stderr, "Error initializing the open list\n");
        return 0;
    }
    if(!closedlist_init(astar))
    {
        fprintf(stderr, "Error initializing the closed list\n");
        return 0;
    }


    // Defining the initial node 
    
    item = malloc(sizeof(anode));
    if(item == NULL)
    {
        return 0; 
    }  
    sprintf(item->id, "%10d%10d", source.x, source.y);
    item->deph = 0;
    item->point = source;
    item->x = source.x;
    item->y = source.y;
    item->h = hdist_mini(item->x, item->y, goal.x, goal.y);
    item->g = 0;
    item->f = item->g + item->h;
    item->parent = NULL;
    


    // Insert the initial node to the open list
    if(!openlist_insert(astar, item))
    {
        fprintf(stderr, "Coudn't add element to the open list!\n");
        return 0;
    }

    while(!openlist_isempty(astar))
    {
        ////////////////////////////////////////////
        // Remove the lowest element in open list
        // and add it to the closed list
        ////////////////////////////////////////////

        COMMENT
            printf("********** New Loop Cycle\n");
            puts("Open List:\n");
            openlist_print(astar);
            puts("Closed List:\n");
            closedlist_print(astar);
        COMMENTEND

        item = openlist_getmin(astar);
        if(item == NULL)
        {
            fprintf(stderr, "Error deleting the priority element from open list!\n");
            return 0;
        }
        if(!closedlist_insert(astar, item))
        {
            fprintf(stderr, "Error adding to hashtable!\n");
            return 0;
        }
        COMMENT
            printf("Removed id: %s\n", item->id);
            printf("Element id to store in loop: %s\n", item->id);
        COMMENTEND  

        // This is the current node
        astar->current_node = item;

        /////////////////////////////////////////////
        // Is this element the goal?
        /////////////////////////////////////////////

        if(item->x == goal.x && item->y == goal.y)
        {
            printf("Solution deph is %d\n", item->deph);
            solution = malloc(item->deph * sizeof(apoint *));

            if(!solution)
            {
                astar->item = NULL;
                return 0;
            }

            i=0;

            while(item->parent)
            {
                //printf("(%d,%d)\n",item->point.x, item->point.y);
                solution[i] = &(item->point);
                item = item->parent;
                i++;
            } 
            
            
            astar->path.size = i - 1;
            astar->path.path = solution;
            astar->item = item;


            return 1;
        }

        COMMENT
            printf("This element is not the goal!.. Trying...\n");
        COMMENTEND
        
        ///////////////////////////////////////////////////////////
        // For each valid move for n, also known as the neigbhors 
        // of the current node.
        // ///////////////////////////////////////////////////////

            //printf("Begin of minimap A*\n");
        for(a = 0; a < NUM_DIRS; a++)
        {
            //printf("Begin of minimap A*\n");

            vector = get_vector_mini(minimap, item->point, a);
            //printf("%d, %d\n", vector.x, vector.y);
            if(vector.x != -2 && vector.y != -2)
            {
                COMMENT
                    printf("For %d direction tile in (%d,%d) is valid?\n", \
                            a, item->point.x, item->point.y);
                    printf("Vector is valid...\n");
                COMMENTEND

                pt.x = vector.x + item->point.x;
                pt.y = vector.y + item->point.y;
                if(ai_valid_tile_minimap(minimap, pt))
                {
                    //New valid element
                    next = malloc(sizeof(anode));
                    next->deph = item->deph + 1;
                    next->point = pt;
                    memset(next->id, 0, 20);
                    sprintf(next->id, "%10d%10d", pt.x, pt.y);
                    next->h = hdist_mini(next->point.x, next->point.y, goal.x,\
                            goal.y);
                    next->x = next->point.x;
                    next->y = next->point.y;
                    if( a == ISO_N || 
                        a == ISO_S ||
                        a == ISO_W ||
                        a == ISO_E)
                        next->g = item->g + G; 
                    else
                        next->g = item->g + G+4;
                    next->f = next->g + next->h; // F = G + H
                    next->parent = item;


                    //printf("Analysing %s\n", next->id);
                    COMMENT
                        printf("Adding direction %d to open list!\n", a);
                        printf("Actual id: %s, H: %d G:%d F:%d Deph:%d\n", \
                               next->id, next->h, next->g, next->f, \
                               next->deph);
                    COMMENTEND

                    open_flag = 0;
                    closed_flag = 0;
                    ///////////////////////////////////////////////
                    // Is this element in closed list?
                    // If the neigbhor is in closed list and 
                    // cost less than g(neigbhor) then remove neighbor
                    // from closed list.
                    //////////////////////////////////////////////
                    if((old = closedlist_search(astar, next)) != NULL)
                    {
                        COMMENT
                            puts("The element is in the closed list!\n");
                        COMMENTEND
                        if(next->f < old->f)
                        {
                            COMMENT
                                printf("Prior element from closed list is \
                                        greater than the next value!\n");
                                printf("Removing %s from closed list and \
                                        adding to open list\n", old->id);
                            COMMENTEND
                            
                            if(!closedlist_remove(astar, old))
                            {
                                printf("Error ocurred while trying to remove \
                                        key in hashtable!\n");
                                COMMENT
                                    closedlist_print(astar);
                                COMMENTEND
                                astar->item = NULL;
                                return 0;
                            }
                            free(old);
                            old = NULL;
                            closed_flag = 0;
                            COMMENT
                                printf("Succesfully removed from closed list \
                                        and added to open list\n");
                            COMMENTEND
                        }
                        else
                        {
                            closed_flag = 1;
                        }
                    }
                    /////////////////////////////////////////////////
                    // Is this element in open list?
                    // If the neigbhor is in openlist and
                    // cost less than g(neighbor) then remove 
                    // neighbor from open list.
                    /////////////////////////////////////////////////
                    if((tmp_item = openlist_search(astar,next))!=NULL)
                    {
                        COMMENT
                            printf("Node exists in openlist!\n");
                        COMMENTEND
                        if(tmp_item->f > next->f && closed_flag == 0)
                        {
                            open_flag = 0;
                            if(openlist_remove(astar, tmp_item))
                            {
                                free(tmp_item);
                                tmp_item = NULL;
                            }
                            COMMENT
                                printf("The new path is better: removing and \
                                        adding the new one!\n");
                            COMMENTEND
                        }
                        else
                        {
                            open_flag = 1;
                        }
                    }
                    ////////////////////////////////////////////////////////////
                    // If the element is not in the open list nor in the closed
                    // we can add the new node to the open list
                    //////////////////////////////////////////////////////////
                    
                    if(open_flag == 0 && closed_flag == 0)
                    {
                        COMMENT
                            printf("Node doesn't exist in closed list or in open\
                                    list!\n");
                        COMMENTEND
                        if(!openlist_insert(astar, next))
                        {
                            printf("Error ocurred while adding a new element to \
                                    open list\n");
                            astar->item = NULL;
                            return 0;
                        }
                    }
                    else
                    {
                        if(next != NULL)
                        {
                            free(next);
                            next = NULL;
                        }
                    }
                }
            }
        }
    }
    astar->item = NULL;
    printf("No path found\n");
    return 0;
}
*/

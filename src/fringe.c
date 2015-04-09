/*
 * fringe.c 
 *
 * Description: home of the fringe_search function, and the 
 *              main algorithm. 
 *             
 * Author: Manuel Mager <fongog@gmail.com> (C) 2014
 * Copyright: GPL v3 or later
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "dllist.h"
#include "base.h"
#include "graphs.h"
#include "fringe.h"

#define INF 9999999999999

int     fs_free(void *node);
int     fs_print(void *node);
int     fs_comp(void *node1, void *node2);
int     h(fobj *fringe, fnode *node);
int     cost(avector vect);

// Functions for the Double Linked List
int fs_comp(void *node1, void *node2)
{
    if(node1 != NULL && node2 != NULL)
    {
        if(((anode *)node1)->f < ((anode *)node1)->f)
            return 1;
        else
            return 0;
    }
    return 0;
}

int fs_print(void *node)
{
    fnode *item;
    fnode *parent;
    if(!node)
        return 0;
    item = (fnode *)node;
    parent = item->parent;
    if(parent)
        printf("x:%d y:%d f:%ld g:%ld Parent-> x:%d y:%d g:%ld\n", item->x, item->y, \
                item->f, item->g, parent->x, parent->y, parent->g);
    else
        printf("x:%d y:%d f:%ld  \n", item->x, item->y, item->f);
    return 1;
}
    
int fs_free(void *node)
{
    /*
    if(node)
    {
        free(node);
        node = NULL;
        return 1;
    }
    */
    return 1;
}

int 
cost(avector vect)
{
    if( (vect.x == 1 && vect.y == 1) || \
        (vect.x == -1 && vect.y == 1) || \
        (vect.x == 1 && vect.y == -1) || \
        (vect.x == -1 && vect.y == -1))
        return 14;
    return 10;
}

// Modified Chebyshev distance to get a precise distance 
// for NW, NE, SW and SE direccions. 
int h(fobj *fringe, fnode *node)
{
    int xDistance = abs(node->x - fringe->xgoal);
    int yDistance = abs(node->y - fringe->ygoal);
    
    if (xDistance > yDistance)
    {
        return 15*yDistance + 10*(xDistance-yDistance);
    }
    else
    {
        return 15*xDistance + 10*(yDistance-xDistance);
    }
}


fnode *fs_getnode(fobj *fringe, int x, int y)
{
    if(!fringe)
    {
        printf("Fringe is empty");
        return NULL;
    }
    if(!fringe->cache)
    {
        printf("The Cache is empy\n");
        return NULL;
    }
    if(fringe->size < COORD(x,y) || 0 > COORD(x,y))
    {
        COMMENT
            printf("The source (%d,%d) is not in the map\n", x, y);
        COMMENTEND
        return NULL;
    }
    else
    {
        if(x<0 || y<0)
        {
            return NULL;
        }
    }
    if(fringe->cache[COORD(x,y)].valid)
    {
        return &(fringe->cache[COORD(x,y)]); 
    }

    return NULL;
}

int fs_movtonow(fobj *fringe, int x, int y)
{

    if(!(fringe->cache[COORD(x,y)].visited))
    {
        dllist_addtonow(fringe->list, &(fringe->listnode_table[COORD(x,y)]));
        fringe->cache[COORD(x,y)].visited = 1;
        return 1;
    }
    return 0;
}

fobj *fs_create(int xsource, int ysource, int xgoal, int ygoal,\
        int xsize, int ysize, char *file) 
{
    int i;
    int x, y;
    fobj *fringe;

    fringe = malloc(sizeof(fobj));
    if(fringe == NULL)
        return NULL;

    fringe->xsource = xsource;
    fringe->ysource = ysource;
    fringe->xgoal = xgoal;
    fringe->ygoal = ygoal;
    fringe->xsize = xsize;
    fringe->ysize = ysize;
    fringe->size = xsize * ysize;
    fringe->list = dllist_new(&fs_comp, &fs_print, &fs_free);
    fringe->result = NULL; 
    sprintf(fringe->output, file);

    if(fringe->list == NULL)
        return NULL;

    fringe->cache = malloc(fringe->size * sizeof(fnode));
    if(fringe->cache == NULL)
        return NULL;

    for(i = 0; i<fringe->size; i++)
    {
        x = i / ysize;
        y = i % ysize;
        fringe->cache[i].visited = 0;
        fringe->cache[i].inlist = 0;
        fringe->cache[i].valid = gmaps[x][y].usable;
        fringe->cache[i].x = x;
        fringe->cache[i].y = y;
        fringe->cache[i].g = 0;

        // This is the part of the algoritm that asign
        // NULL to the parent pointer of all members of 
        // the cache. 
        //    C[n] <- null for n != start
        fringe->cache[i].parent = NULL;
    }

    fringe->listnode_table = malloc(fringe->size * sizeof(dlnode));
    if(!(fringe->listnode_table))
        return NULL;
    for(i = 0; i < fringe->size; i++)
    {
        fringe->listnode_table[i].next = NULL;
        fringe->listnode_table[i].prev = NULL;
        fringe->listnode_table[i].data = &(fringe->cache[i]);
    }

    for(i = 0; i<fringe->size; i++)
    {
        fringe->cache[i].lnode = &(fringe->listnode_table[i]);
    }


    return fringe;
}

int fs_free_fobj(fobj *fringe)
{
    int i;
    
    free(fringe->cache);
    free(fringe->listnode_table);
    fringe->cache = NULL;
    fringe->listnode_table = NULL;
    fringe->result = NULL; 
    free(fringe->list);
    free(fringe);
    fringe = NULL;


    return 1;
}

int fringe_search(fobj *fringe)
{
    long        flimit, fmin, g, gs, gi, f;
    int         found, i; 
    avector     vect;
    dlnode      *nnode, *tmp_node;
    fnode       *start, *n, *goal, *s;


    goal = &(fringe->cache[COORD(fringe->xgoal, fringe->ygoal)]);
    start = &(fringe->cache[COORD(fringe->xsource, fringe->ysource)]);
    
    n = fs_getnode(fringe, fringe->xgoal, fringe->ygoal);
    if(!n)
    {
        printf("Goal is not valid.\n");
        return 0;
    }
    n = fs_getnode(fringe, fringe->xsource, fringe->ysource);
    if(!n)
    {
        printf("Soruce is not valid.\n");
        return 0;
    }
    printf("Source: (%d,%d) Goal: (%d,%d)\n", fringe->xsource, fringe->ysource, \
            fringe->xgoal, fringe->ygoal);
    n = NULL;

    /////////////////
    // Initialize: //
    /////////////////

    // Fringe<-(s)
    fs_movtonow(fringe, fringe->xsource, fringe->ysource);
    start->inlist = 1;
    
    // Cache C[start]<-(0, null)

    fringe->cache[COORD(fringe->xsource, fringe->ysource)].g = 0;
    fringe->cache[COORD(fringe->xsource, fringe->ysource)].parent = NULL;

    // C[n]<-null for n != start
    // is already done in fs_create()
    
    //flimit<-h(start)
    flimit = h(fringe, start);
    COMMENT
        printf("flimit = %ld\n", flimit);
    COMMENTEND

    //found<-false
    found = 0;


    //////////////////////////////////////////
    // Repeat until found = true or F empty //
    //////////////////////////////////////////
    while(!found && !dllist_isempty(fringe->list))
    {
        //fmin<-infinte
        fmin = INF;
        nnode = fringe->list->start;

        //Iterate over nodes n in F from left to right
        do
        {

            n = (fnode *)(nnode->data);
            if(!n)
            {
                printf("n is empty\n");
                return 0;
            }


            //(g, parent)<-c[n]            
            g = n->g; 

            //This operation is not needed, but defined as formal
            //in the algorithm.
            //parent = n->parent;

            // f <- g + h(n)
            f = g + h(fringe, n);

            
            COMMENT
                if(n->parent)
                {
                    printf("%ld: (%d,%d) Parent-> (%d,%d) g:%ld\n",f, n->x, n->y, \
                        n->parent->x, n->parent->y, n->parent->g);
                }
                else
                {
                   printf("%ld: (%d,%d)\n",f, n->x, n->y);
                }
            COMMENTEND
            // If f>flimit
            //   fmin<-min(f,fmin)
            //   continue
            COMMENT
                printf("%ld > %ld\n", f, flimit);
            COMMENTEND
            if(f>flimit)
            {
                COMMENT
                    printf("To later!\n");
                COMMENTEND
                fmin = min(f, fmin);
                nnode = nnode->next;
                continue;
            }
            // If n = goal
            //   found<-true
            //   break
            if(n == goal)
            {
                printf("Goal Found!\n");
                found = 1;
                break;
            }
            //Iterate over s in successors(n) from right to left:
            for(i = 7; i >= 0; i--)
            {
                vect = get_iso_vector(i);
                s = fs_getnode(fringe, (vect.x + n->x), (vect.y + n->y));
                if(s == NULL)
                {
                    continue;
                }

                // gs<-g+cost(n,s)
                gs = g + cost(vect);

                // If C[s] != null
                //   (g',parent)<-C[s]
                //   If gs >= g'
                //      continue
                if(s->visited)
                {
                    COMMENT
                        printf("Node visited: (%d,%d)\n", s->x, s->y);
                    COMMENTEND
                    gi = s->g;
                    if(gs >= gi)
                    {
                        COMMENT
                            printf("Continue\n");
                        COMMENTEND
                        continue;
                    }
                    COMMENT
                        printf("Go ahead\n");
                    COMMENTEND
                }

                // If F contains s
                //   remove s from F
                if(s->inlist)
                {
                    COMMENT
                        printf("Node already in list: (%d,%d)\n", s->x, s->y);
                    COMMENTEND
                    dllist_removefromlist(fringe->list, s->lnode); 
                    s->inlist = 0;
                }

                // Insert s into F after n
                s->visited = 1;
                s->g = gs;
                s->parent = n;
                s->f = gs + h(fringe, s);
                COMMENT
                    printf("Insert: (%d,%d)\n", s->x, s->y);
                    printf("New g: %ld = %ld + %d h: %d f: %ld\n", gs, g, cost(vect), \
                            h(fringe,s), s->f);
                    printf("parent -> x:%d y:%d g:%ld\n", s->parent->x, s->parent->y,\
                          s->parent->g);
                COMMENTEND
                dllist_insertafter(fringe->list, s->lnode, n->lnode);
                s->inlist = 1;
                COMMENT
                    printf("--------------\n");
                    //dllist_print(fringe->list, nnode);
                COMMENTEND
            }

            tmp_node = nnode->next;
            // Remove n from F
            COMMENT
                printf("Remove (%d,%d) from list\n", n->x, n->y);
                if(!tmp_node)
                    printf("Next node is NULL!\n");
                //dllist_print(fringe->list, nnode);
            COMMENTEND
            dllist_removefromlist(fringe->list, nnode); 
            n->inlist = 0;
            nnode = tmp_node;
       }while(nnode != NULL);

        // flimit <- fmin
        flimit = fmin;
        COMMENT
            printf("*********************\n");
        COMMENTEND
    }
    // If found = true
    //   Construct path from parent nodes in cache
    if(found)
    {
        fringe->result = n;
        while(n)
        {
            COMMENT
                printf("(%d,%d)\n", n->x, n->y);
            COMMENTEND
            n = n->parent;
        }
        return 1;
    }
    return 0;
}




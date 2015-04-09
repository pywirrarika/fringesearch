#include<stdio.h>
#include<stdlib.h>

#include "dllist.h"
#include "fringe.h"

dllist  *
dllist_new(dl_comparator comp, dl_print print, dl_freefunc ff)
{
    dllist *list;

    if(comp && print && ff)
    { 
        list = malloc(sizeof(dllist));
        list->comp = comp;
        list->print = print;
        list->free_func = ff;
        list->start = NULL;
        list->end = NULL;

        return list;
    }
    return NULL;
}


int     
dllist_isempty(dllist *list)
{
if(!list)
    return 1;
if(list->start == NULL)
    return 1;
else
    return 0;
}

int
dllist_addtonow(dllist *list, dlnode *node)
{
    dlnode *tmp;

    if(!(list && node))
        return 0;

    if(dllist_isempty(list))
    {
        list->start = node;
        list->end = node;
        node->next = NULL;
        node->prev = NULL;
    }
    else
    {
        tmp = list->end;
        list->end = node;
        node->next = NULL;
        node->prev = tmp;
        tmp->next = node;
    }
    return 1;
}

int
dllist_removefromlist(dllist *list, dlnode *node)
{
    dlnode *tmp1, *tmp2;

    if(!(list || node))
        return 0;

    tmp1 = node->next;
    tmp2 = node->prev;

    if(tmp2)
        tmp2->next = tmp1;
    if(tmp1)
        tmp1->prev = tmp2;

    if(node == list->start)
        list->start = node->next;

    if(node == list->end)
        list->end = node->prev;

    node->next = NULL;
    node->prev = NULL;
    return 1;
}

int
dllist_insertafter(dllist *list, dlnode *node, dlnode *after)
{
    dlnode *tmp;

    if(!(list || node || after))
        return 0;

    if(list->end == after)
    {
        list->end = node;
    }
    tmp = after->next;
    after->next = node;
    node->next = tmp;
    node->prev = after;
    if(tmp)
        tmp->prev = node;

    return 1;
}

void    
dllist_print(dllist *list, dlnode *n)
{
    int i = 0;
    dlnode *node;

    if(list)
    {
        if(list->start)
        {
            node = list->start;
        }
        else
        {
            printf("List empty.\n"); 
            return;
        }
    }
    else
    {
        printf("List empty.\n"); 
        return;
    }

    while(node != NULL)
    {
        printf("%d: ",i);
        if(node == list->start)
            printf("[START] ");
        if(node == list->end)
            printf("[END] ");
        if(node == n)
            printf("-> ");
        list->print(node->data);
        node = node->next;
        i++;
    }
    return;
}


int
dllist_free(dllist *list){
    dlnode *node, *tmp;;

    if(!list)
        return 0;

    node = list->start;
    while(node)
    {
        tmp = node->next;
        // the void *data points to an element in array fringe->cache,
        // so we dont need to free void *data here. 
        free(node);
        node = tmp;
    }
    free(list);
    list = NULL;

    return 1;
}

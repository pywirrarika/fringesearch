/*
 * base.c
 *
 * Description: Functions to handle the base structures. 
 *
 * Author: Manuel Mager <fongog@gmail.com> (C) 2014
 * Copyright: GPL v3 or later
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

void free_astar(aobj *free)
{
    return;
}

//An simple function to add one character at the end of a string

char *str_add(char *str, char add)
{
    size_t len  = strlen(str);
    char *str2  = malloc(len + 1 + 1);
    strcpy(str2, str);
    str2[len-2] = add;
    str2[len-1] = '\0';

    return str2;
}

    

    

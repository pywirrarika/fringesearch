/*
 * paral_tools.h 
 *
 * Description: Tools that will help us making the algorithm 
 *              parallel. 
 *
 * Author: Manuel Mager <fongog@gmail.com> (C) 2014
 * Copyright: GPL v3 or later
 *
 */

#ifndef PARAL_TOOLS
#define PARAL_TOOLS

#include "base.h"
#include "graphs.h"
#include "dllist.h"

int paral_check_col(aobj *one, aobj *two);
gnode **paral_small_map(apoint, apoint);
void free_minimap(gmini *minimap);
aparallpath *paral_create_patharray(int num_proc);
#endif

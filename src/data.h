/*
 * data.h 
 *
 * Description: Arrays containing the maps to use with A*.
 *
 * Author: Manuel Mager <fongog@gmail.com> (C) 2013
 * Copyright: GPL v3 or later
 *
 */
#ifndef DATA_H
#define DATA_H

#include <stdio.h>
#include <string.h>

#include "qdbmp.h"
#include "base.h"
#include "fringe.h"

int read_map(char *);

int data_store(fobj *);
int store_map(fobj *, char*);
#endif

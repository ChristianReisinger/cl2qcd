#ifndef _GEOMETRYH_
#define _GEOMETRYH_

#include "globaldefs.h"
#include "hmcerrs.h"
#include "cstdlib"
#include "cmath"

//coord[0] = t
//coord[1] = x
//coord[2] = y
//coord[3] = z

int get_n_eoprec(int timepos, int spacepos);

//switch between (x,y,z) <-> nspace=0,...,VOLSPACE-1
int get_nspace(int* coord);
int get_spacecoord(int nspace, int dir);

int get_neighbor(int nspace, int dir);
int get_lower_neighbor(int nspace, int dir);

int get_ntime_from_eoprecindex(int n, int which);
int get_nspace_from_eoprecindex(int n, int which);

extern int* nspace_from_even_index;
extern int* ntime_from_even_index;
extern int* nspace_from_odd_index;
extern int* ntime_from_odd_index;

#endif

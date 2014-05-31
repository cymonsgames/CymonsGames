/* Basic-level type definitions */
#ifndef __COORD_H
#define __COORD_H

typedef int boolean;
typedef unsigned char uchar8;
typedef unsigned short uchar16;
typedef signed short int16;
typedef signed long int32;

/* Basic coordinate type */
typedef struct sCoordinate {
    int x;
    int y;
} COORD;

#endif

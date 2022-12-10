#ifndef QUERYTAD
#define QUERYTAD
#include <stdlib.h>
#include <stdio.h>

typedef QueryCDT * QueryADT;

typedef struct sensor {
    char * name; 
    char flag;
    size_t Namelen;
    long Tpedestrians;
}TSensor; // incluimos esta estructura en el .h para usarla tanto en el main como en el .c


QueryADT newQuery();
#endif
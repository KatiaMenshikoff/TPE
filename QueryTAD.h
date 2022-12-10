#ifndef QUERYTAD
#define QUERYTAD
#include <stdlib.h>
#include <stdio.h>

typedef struct sensor {
    char * name; 
    char flag;
    size_t len;
    long totalPedes;
}TSensor; // incluimos esta estructura en el .h para usarla tanto en el main como en el .c



#endif
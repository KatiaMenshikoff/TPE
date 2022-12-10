#ifndef QUERYTAD
#define QUERYTAD
#include <stdlib.h>

typedef struct sensor {
    char * name; 
    char flag;
    size_t len;
}TSensor; // incluimos esta estructura en el .h para usarla tanto en el main como en el .c

typedef struct
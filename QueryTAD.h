#ifndef QUERYTAD
#define QUERYTAD
#include <stdio.h>
#include <stdlib.h>
#define DIM_SENS 85

typedef struct QueryCDT *QueryADT;

typedef enum days {
  Monday = 0,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
  Sunday
} day;

typedef struct sensor {
  char *name;
  char flag;
  size_t Namelen;
  long Tpedestrians;
} TSensor; // incluimos esta estructura en el .h para usarla tanto en el main
           // como en el .c

typedef struct year {
  char year[4];
  long Dweek;    // peatones anuales en día de semana.
  long Dweekend; // peatones anuales en fin de semana.
  long total;
  struct year *next; // siguiente año porque vamos a armar una lista de años.
} Tyear;

QueryADT newQuery();
#endif

#ifndef QUERYTAD
#define QUERYTAD
#include <stdio.h>
#include <stdlib.h>
#define DIM_SENS 86

enum week {
  Monday = 0,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
  Sunday
};

typedef struct sensor {
  char *name;
  char flag;
  size_t Namelen;
  long Tpedestrians;
} TSensor; // incluimos esta estructura en el .h para usarla tanto en el main
           // como en el .c

typedef struct year {
  size_t year;
  long Dweek;    // peatones anuales en día de semana.
  long Dweekend; // peatones anuales en fin de semana.
  long total;
  struct year *next; // siguiente año porque vamos a armar una lista de años.
} Tyear;

typedef struct node {
  size_t id;
  long int pedestrians;
  struct node *tail;
} TNode;

typedef TNode *TList;

typedef struct QueryCDT *QueryADT;

QueryADT newQuery();

void insertVector(QueryADT q, TSensor v[]);

void insertList(QueryADT q, Tyear *l);

void createList(TSensor sensor[], QueryADT q);

void query1(QueryADT q);

void query2(QueryADT q);

void query3(QueryADT q);

void freeQuery(QueryADT q);

#endif

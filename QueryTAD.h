#ifndef QUERYTAD
#define QUERYTAD
#include <stdio.h>
#include <stdlib.h>
#include "htmlTable.h"
#define DIM_SENS 86

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

typedef Tyear *TListYear;

typedef struct node {
  size_t id;
  long int pedestrians;
  struct node *tail;
} TNode;

typedef TNode *TList;

typedef struct QueryCDT *QueryADT;

//Crea una nueva estructura para almacenar los datos de las queries.
QueryADT newQuery(void);

//inserta el vector de sensores creado a partir de los archivos en el cdt.
void insertVector(QueryADT q, TSensor v[]);

//inserta la lista de años en el cdt.
void insertList(QueryADT q, Tyear *l);

//crea una lista que ordena los sensores por cantidad de peatones.
void createList(QueryADT q, TSensor sensor[]);

//carga la inforación al csv y html del query 1.
void Query1(QueryADT q, FILE * query1, htmlTable table);

//carga la inforación al csv y html del query 2.
void Query2(QueryADT q,FILE * query2, htmlTable table2);

//carga la inforación al csv y html del query 3.
void Query3(QueryADT q,FILE * query3, htmlTable table3);

//libera la memoria utilizada parel cdt.
void freeQuery(QueryADT q);



#endif

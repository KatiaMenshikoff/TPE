#include "QueryTAD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct QueryCDT {
  TSensor *vecSen;
  Tyear *first;         // usamos lista para guardar los años en orden.
  TSensor *firstSensor; // lista de sensores ordenados por cantidad de peatones.
} QueryCDT;

QueryADT newQuery() { return calloc(1, sizeof(QueryCDT)); }

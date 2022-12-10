#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "QueryTAD.h"

typedef struct year{
    char year[4];
    long Dweek; // peatones anuales en día de semana.
    long Dweekend; // peatones anuales en fin de semana.
    long total;
    struct year * next; // siguiente año porque vamos a armar una lista de años.
}Tyear;


typedef struct {
    TSensor * vecSen;
    Tyear * first; // usamos lista para guardar los años en orden.
    TSensor * firstSensor;// lista de sensores ordenados por cantidad de peatones. 
}QueryCDT;


QueryADT newQuery(){
    return calloc(1, sizeof(QueryCDT));
}


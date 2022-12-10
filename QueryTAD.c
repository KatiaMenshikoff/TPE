#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "QueryTAD.h"


typedef struct {
    TSensor * vecSen;
    Tyear * first; // usamos lista para guardar los años en orden.
    TSensor * firstSensor;// lista de sensores ordenados por cantidad de peatones. 
}QueryCDT;


QueryADT newQuery(){
    return calloc(1, sizeof(QueryCDT));
}




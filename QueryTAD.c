#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "QueryTAD.h"

typedef struct year{
    char year[4];
    long week;
    long weekend;
    long total;
}Tyear;


typedef struct {
    TSensor * vecSen;
    long int total;

}QueryCDT;





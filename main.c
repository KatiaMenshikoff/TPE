#include "QueryTAD.h"
#include <ctype.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>
#define MAX_LINE 400

//enum week {Monday = 0,Tuesday,Wednesday,Thursday,Friday,Saturday,Sunday};

Tyear *makeList(FILE *fReadings, TSensor vecSensors[]);
TSensor *makeVec(FILE *fSensor, TSensor vecSensors[]);
Tyear *makeRec(Tyear *l, size_t year, bool day, int ID, int pedestrians);

int main(int argc, char *argv[]) {
  FILE *fSensor = fopen(argv[1], "rt");
  FILE *fReadings;
  fReadings = fopen(argv[2], "rt");
  if (fSensor == NULL || fReadings == NULL) {
    perror("Unable to open the file.");
    exit(1);
  }
  
  QueryADT query = newQuery();
  TSensor * vecSensors = calloc(DIM_SENS, sizeof(TSensor)); 
  vecSensors = makeVec(fSensor, vecSensors);
  fclose(fSensor);
  insertVector(query, vecSensors);
  Tyear * list = makeList(fReadings, vecSensors);
  fclose(fReadings);
  insertList(query, list);
  createList(vecSensors, query);
  query1(query);
  query2(query);
  query3(query);
  freeQuery(query);
  return 0;
}

static bool dayToNum(char s[]){
  return s[0] == 'S' || s[0] == 's';
}

TSensor *makeVec(FILE *fSensor, TSensor vecSensors[]) {
  char line[MAX_LINE];
  while (!feof(fSensor)) {
    for (int i = 0; fgets(line, MAX_LINE, fSensor); i++) {
      if (i == 0) { 
        continue;
      } else {
        char *value = strtok(line, ";");
        while (value != NULL) {
          size_t pos = atoi(value);
          value = strtok(NULL, ";");
          vecSensors[pos - 1].Namelen = strlen(value);
          vecSensors[pos - 1].name = malloc(vecSensors[pos - 1].Namelen + 1);
          if (vecSensors[pos - 1].name == NULL) {
            perror("Unable to allocate memory.");
            exit(1);
          }
          vecSensors[pos - 1].name = strcpy(vecSensors[pos - 1].name, value);
          value = strtok(NULL, ";");
          vecSensors[pos - 1].flag = *value;
          value = strtok(NULL, ";");
        }
      }
    }
  }
  return vecSensors;
}

Tyear * makeList(FILE *fReadings, TSensor vecSensors[]) {
  Tyear *list = NULL;
  char line2[MAX_LINE];
  while (!feof(fReadings)) {
    for (int i = 0; fgets(line2, MAX_LINE, fReadings); i++) {
      if (i == 0) { 
        continue;
      } else {
        char *value = strtok(line2, ";");
        while (value != NULL) {
          size_t year = atoi(value);
          //printf("YEAR: %lu\t", year);
          value = strtok(NULL, ";");
          value = strtok(NULL, ";");
          value = strtok(NULL, ";");
          bool day = dayToNum(value);
          //printf("DAY: %d\t", day);
          value = strtok(NULL, ";");
          int ID = atoi(value);
          //printf("ID: %i\t", ID);
          value = strtok(NULL, ";");
          value = strtok(NULL, ";");
          int pedestrians = atoi(value);
          //printf("PED: %i\n", pedestrians);
          vecSensors[ID - 1].Tpedestrians += pedestrians;
          list = makeRec(list, year, day, ID, pedestrians);
          value = strtok(NULL, ";");
        }
      }
    }
  }
  return list;
}

/*Función que crea una lista de años con cantidad de peatones*/
Tyear *makeRec(Tyear *l, size_t year, bool day, int ID, int pedestrians) {
  if (l == NULL || l->year > year) {
    Tyear *aux = malloc(sizeof(Tyear));
    if (aux == NULL) {
      perror("Unable to allocate memory.");
      exit(1);
    }
    if (!day) {
      aux->Dweek = pedestrians;
      aux->Dweekend = 0;
    } else {
      aux->Dweekend = pedestrians;
      aux->Dweek = 0;
    }
    aux->total = pedestrians;
    aux->next = l;
    aux->year = year;
    return aux;
  }
  if (l->year == year) {
    if (!day) {
      l->Dweek += pedestrians;
    } else {
      l->Dweekend += pedestrians;
    }
    l->total += pedestrians;
    return l;
  }
  l->next = makeRec(l->next, year, day, ID, pedestrians);
  return l;
}

#include "QueryTAD.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#define MAX_LINE 400

// enum week {Monday = 0,Tuesday,Wednesday,Thursday,Friday,Saturday,Sunday};

Tyear *makeList(FILE *fReadings, TSensor *vecSensors[]);
TSensor *makeVec(FILE *fSensor, TSensor * vecSensors);
Tyear *makeRec(Tyear *l, size_t year, bool day, int ID, int pedestrians,
               char flag);

int main(int argc, char *argv[]) {
  FILE *fSensor = fopen(argv[1], "rt");
  FILE *fReadings;
  fReadings = fopen(argv[2], "rt");
  if (fSensor == NULL || fReadings == NULL) {
    perror("Unable to open the file.");
    exit(1);
  }
  QueryADT query = newQuery();
  TSensor *vecSensors = calloc(DIM_SENS, sizeof(struct sensor));
  vecSensors = makeVec(fSensor, vecSensors);
  Tyear *list = makeList(fReadings, &vecSensors);
  insertVector(query, vecSensors);
  insertList(query, list);
  createList(query, vecSensors);
  fclose(fSensor);
  fclose(fReadings);

  for (int i = 0; i < DIM_SENS; i++) {
    if(vecSensors[i].name == NULL){
      continue;
    } else{
        free(vecSensors[i].name);
    }
  }
  free(vecSensors);
  Query1(query);
  Query2(query);
  Query3(query);
  freeQuery(query);
}

static bool dayToNum(char s[]) { return s[0] == 'S' || s[0] == 's'; }

//crea un vector de sensores ordenados por ID.
TSensor *makeVec(FILE *fSensor,TSensor *vecSensors) {
  char line[MAX_LINE];
  if (vecSensors == NULL) {
    perror("Unable to allocate memory.");
    exit(1);
  }
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

//crea una lista de años ordenados de mayor a menor.
Tyear *makeList(FILE *fReadings, TSensor *vecSensors[]) {
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
          value = strtok(NULL, ";");
          value = strtok(NULL, ";");
          value = strtok(NULL, ";");
          bool day = dayToNum(value);
          value = strtok(NULL, ";");
          int ID = atoi(value);
          value = strtok(NULL, ";");
          value = strtok(NULL, ";");
          int pedestrians = atoi(value);
          (*vecSensors)[ID - 1].Tpedestrians += pedestrians;
          list = makeRec(list, year, day, ID, pedestrians,
                         (*vecSensors)[ID - 1].flag);
          value = strtok(NULL, ";");
        }
      }
    }
  }
  return list;
}

// Crea la lista de años recursivamente.
Tyear *makeRec(Tyear *l, size_t year, bool day, int ID, int pedestrians,
               char flag) {
  if (flag == 'R') {
    return l;
  }
  if (l == NULL || l->year < year) {
    Tyear *aux = malloc(sizeof(struct year));
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
  l->next = makeRec(l->next, year, day, ID, pedestrians, flag);
  return l;
}

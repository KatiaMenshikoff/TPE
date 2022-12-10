#include "QueryTAD.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tyear *makeRec(Tyear *l, char year[], int day, int ID, int pedestrians, TSensor sensors[]);

int main(int argc, char *argv[]) {
  FILE * fSensor = fopen(argv[1], "rt");
  char line[100];
  if (fSensor == NULL) {
    perror("Unable to open the file.");
    exit(1);
  }
  TSensor vecSensors[DIM_SENS];
  while (!feof(fSensor)) {
    for (int i = 0; fgets(line, 100, fSensor); i++) {
      if (i == 0) { // CAMBIAR
        continue;
      } else {
        char *value = strtok(line, "; ");
        while (value != NULL) {
          size_t pos = atoi(value);
          value = strtok(NULL, "; ");
          vecSensors[pos - 1].Namelen = strlen(value);
          vecSensors[pos - 1].name = malloc(vecSensors[pos - 1].Namelen + 1);
          vecSensors[pos - 1].name = strcpy(vecSensors[pos - 1].name, value);
          value = strtok(NULL, "; ");
          vecSensors[pos - 1].flag = *value;
          value = strtok(NULL, "; ");
          vecSensors[pos - 1].Tpedestrians = 0;
        }
      }
    }
  }
  fclose(fSensor);

  QueryADT query = newQuery();
  insertVector(query, vecSensors);

  Tyear *list;
  FILE *fReadings;
  fReadings = fopen(argv[2], "rt");
  char line2[100];
  if (fReadings == NULL) {
    perror("Unable to open the file.");
    exit(1);
  }
  while (!feof(fReadings)) {
    for (int i = 0; fgets(line2, 100, fReadings); i++) {
      if (i == 0) { // CAMBIAR
        continue;
      } else {
        char *value = strtok(line2, "; ");
        while (value != NULL) {
          char year[4];
          strcpy(year, value);
          value = strtok(NULL, "; ");
          value = strtok(NULL, "; ");
          value = strtok(NULL, "; ");
          int day = atoi(value);
          value = strtok(NULL, "; ");
          int ID = atoi(value);
          value = strtok(NULL, "; ");
          value = strtok(NULL, "; ");
          int pedestrians = atoi(value);
          list = makeRec(list, year, day, ID, pedestrians, vecSensors);
        }
      }
    }
  }
  fclose(fReadings);

  insertList(query, list);
  createList(vecSensors, query);
  query1(query);
  return 0;
}




/*Función que crea una lista de años con cantidad de peatones*/
Tyear *makeRec(Tyear *l, char year[], int day, int ID, int pedestrians, TSensor sensors[]) {
  if (l == NULL || strcmp(l->year, year) > 0) {
    Tyear *aux = malloc(sizeof(Tyear));
    if (day < 5) {
      aux->Dweek = pedestrians;
      aux->Dweekend = 0;
    } else {
      aux->Dweekend = pedestrians;
      aux->Dweek = 0;
    }
    aux->total = pedestrians;
    aux->next = l;
    strcpy(aux->year, year);
    return aux;
  }
  if (strcmp(l->year, year) == 0) {
    if (day < 5) {
      l->Dweek += pedestrians;
    } else {
      l->Dweekend += pedestrians;
    }
    l->total += pedestrians;
    return l;
  }
  l->next = makeRec(l->next, year, day, ID, pedestrians, sensors);
  return l;
}

Tyear *makeYearList(TSensor sensors[]) {
  Tyear *list;
  FILE *fReadings;
  fReadings = fopen("sensor.csv", "rt");
  char line[100];
  if (fReadings == NULL) {
    perror("Unable to open the file.");
    exit(1);
  }
  while (!feof(fReadings)) {
    for (int i = 0; fgets(line, 100, fReadings); i++) {
      if (i == 0) { // CAMBIAR
        continue;
      } else {
        char *value = strtok(line, "; ");
        while (value != NULL) {
          char year[4];
          strcpy(year, value);
          value = strtok(NULL, "; ");
          value = strtok(NULL, "; ");
          value = strtok(NULL, "; ");
          int day = atoi(value);
          value = strtok(NULL, "; ");
          int ID = atoi(value);
          value = strtok(NULL, "; ");
          value = strtok(NULL, "; ");
          int pedestrians = atoi(value);
          list = makeRec(list, year, day, ID, pedestrians, sensors);
        }
      }
    }
  }
  fclose(fReadings);
  return list;
}



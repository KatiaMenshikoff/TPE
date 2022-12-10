#include "QueryTAD.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  QueryADT query;
  query = newQuery();
  query->vecSen = makeVec();  //creamos un vector de sensores
  query->first = makeYearList(query->vecSen);  //creamos una lista ordenada por año
  query->sensors = createList(query->vecSen);
  return 0;
}

/*Función que crea un vector de sensores ordenados por ID*/
TSensor *makeVec(void) {
  TSensor ans[DIM_SENS];
  FILE *fSensor;
  fSensor = fopen("sensor.csv", "rt");
  char line[100];
  if (fSensor == NULL) {
    perror("Unable to open the file.");
    exit(1);
  }
  while (!feof(fSensor)) {
    for (int i = 0; fgets(line, 100, fSensor); i++) {
      if (i == 0) {  //CAMBIAR
        continue;
      } else {
        char *value = strtok(line, "; ");
        while (value != NULL) {
          size_t pos = atoi(value);
          value = strtok(NULL, "; ");
          ans[pos - 1].Namelen = strlen(value);
          ans[pos - 1].name = malloc(ans[pos - 1].Namelen + 1);
          ans[pos - 1].name = strcpy(ans[pos - 1].name, value);
          value = strtok(NULL, "; ");
          ans[pos - 1].flag = *value;
          value = strtok(NULL, "; ");
          ans[pos - 1].Tpedestrians = 0;
        }
      }
    }
  }
  fclose(fSensor);
  return ans;
}

/*Función que crea una lista de años con cantidad de peatones*/
static Tyear *makeRec(Tyear *l, char year[], int day, int ID, int pedestrians,
                      TSensor sensors[]) {
  if (l == NULL || strcmp(l->year, year) > 0) {
    Tyear *aux = malloc(sizeof(Tyear));
    if (day < 5) {
      aux->Dweek += pedestrians;
    } else {
      aux->Dweekend += pedestrians;
    }
    aux->total += pedestrians;
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
  Tyear *new;
  FILE *fReadings;
  fReadings = fopen("sensor.csv", "rt");
  char line[100];
  if (fReadings == NULL) {
    perror("Unable to open the file.");
    exit(1);
  }
  while (!feof(fReadings)) {
    for (int i = 0; fgets(line, 100, fReadings); i++) {
      if (i == 0) {  //CAMBIAR
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
          new = makeRec(new, year, day, ID, pedestrians, sensors);
        }
      }
    }
  }
  fclose(fReadings);
  return new;
}

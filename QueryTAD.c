#include "QueryTAD.h"
#include "htmlTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct QueryCDT {
  TSensor vecSen[DIM_SENS];
  Tyear *first; // usamos lista para guardar los años en orden.
  TList sensors;
} QueryCDT;

QueryADT newQuery() {
  QueryADT new = calloc(1, sizeof(QueryCDT));
  return new;
}

void insertVector(QueryADT q, TSensor v[]) {
  for (int i = 0; i < DIM_SENS; i++) {
    q->vecSen[i].flag = v[i].flag;
    q->vecSen[i].Namelen = v[i].Namelen;
    q->vecSen[i].name = malloc(q->vecSen[i].Namelen + 1);
    if (!v) {
      fprintf(stderr, "Memory allocation failed!\n");
      exit(1);
    }
    strcpy(q->vecSen[i].name, v[i].name);
    q->vecSen[i].Tpedestrians = v[i].Tpedestrians;
  }
}

void insertList(QueryADT q, Tyear *l) { q->first = l; }

TList addRec(TList list, size_t id, long int peds) {
  if (list == NULL || peds > list->pedestrians) {
    TList aux = malloc(sizeof(TNode));
    aux->id = id;
    aux->pedestrians = peds;
    aux->tail = list;
    return aux;
  }
  list->tail = addRec(list->tail, id, peds);
  return list;
}

void createList(TSensor sensor[], QueryADT q){
  int i;
  TList ans;
  for (i = 0; i < DIM_SENS; i++) {
    ans = addRec(ans, i + 1, sensor[i].Tpedestrians);
  }
  if (ans == NULL) {
    perror("Unable to copy information.");
    exit(1);
  }
  q->sensors = ans;
}



size_t query1(QueryADT q) {
  FILE *query1 = fopen("query1.csv", "wt");
  fprintf(query1, "Sensor, Pedestrians\n");
  while (q->sensors != NULL) {
    fprintf(query1, "%s, %ld\n", q->vecSen[q->sensors->id - 1].name,
            q->sensors->pedestrians);
    q->sensors = q->sensors->tail;
  }
  fclose(query1);
  return 1; // ALGO
}

/*size_t query2(QueryADT q) {
  FILE *query2 = fopen("query2.csv", "wt");
  fprintf(query2, "Year, Weekdays_Count, Weekends_Count, Total_Count\n");
  while (q->first != NULL) {
    fprintf(query2, "%s, %ld, %ld, %ld\n", q->first->year, q->first->Dweek,
            q->first->Dweekend, q->first->total);
    q->first = q->first->next;
  }
  fclose(query2);
  return 1; // ALGO
}

size_t query3(QueryADT q) {
  FILE *ansQuery3 = fopen("query3.csv", "wt");
  fprintf(ansQuery3, "Year, Pedestrians_Avg");
  while (q->first != NULL) {
    if (atoi(q->first->year) % 4 == 0) {
      fprintf(ansQuery3, "%s, %.2f\n", q->first->year,
              (float)q->first->total / 366.0);
    } else {
      fprintf(ansQuery3, "%s, %.2f\n", q->first->year,
              (float)q->first->total / 365.0);
    }
    q->first = q->first->next;
  }
  fclose(ansQuery3);
  return 1; // ALGO
}*/

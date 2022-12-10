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
       perror("Not able to allocate memory.");
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
    if(aux == NULL){
       perror("Not able to allocate memory.");
       exit(1);
    }
    aux->id = id;
    aux->pedestrians = peds;
    aux->tail = list;
    return aux;
  }
  list->tail = addRec(list->tail, id, peds);
  return list;
}

void createList(TSensor sensor[], QueryADT q) {
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

void query1(QueryADT q) {
  FILE *query1 = fopen("query1.csv", "wt");
  htmlTable table = newTable("query1.html", 2, "Sensor", "Pedestrians");
  fprintf(query1, "Sensor, Pedestrians\n");
  while (q->sensors != NULL) {
    fprintf(query1, "%s, %ld\n", q->vecSen[q->sensors->id - 1].name,
            q->sensors->pedestrians);
    addHTMLRow(table, q->vecSen[q->sensors->id - 1].name, q->sensors->pedestrians);
    q->sensors = q->sensors->tail;
  }
  closeHTMLTable(table);
  fclose(query1);
}

void query2(QueryADT q) {
  FILE *query2 = fopen("query2.csv", "wt");
  htmlTable table = newTable("query2.html", 4, "Year", "Weekdays Count", "Weekends Count", "Total Count");
  fprintf(query2, "Year, Weekdays Count, Weekends Count, Total Count\n");
  while (q->first != NULL) {
    fprintf(query2, "%s, %ld, %ld, %ld\n", q->first->year, q->first->Dweek, q->first->Dweekend, q->first->total);
    addHTMLRow(table, q->first->year, q->first->Dweek, q->first->Dweekend, q->first->total);
    q->first = q->first->next;
  }
  fclose(query2);
  closeHTMLTable(table);
}

void query3(QueryADT q) {
  FILE *ansQuery3 = fopen("query3.csv", "wt");
  htmlTable table = newTable("query2.html", 2, "Year", "Pedestrians Avg");
  fprintf(ansQuery3, "Year, Pedestrians Avg");
  while (q->first != NULL) {
    if (atoi(q->first->year) % 4 == 0) {
      fprintf(ansQuery3, "%s, %.2f\n", q->first->year, (float)q->first->total / 366.0);
      addHTMLRow(table, q->first->year,(float)q->first->total / 366.0);
    } else {
      fprintf(ansQuery3, "%s, %.2f\n", q->first->year,(float)q->first->total / 365.0);
      addHTMLRow(table, q->first->year, (float)q->first->total / 365.0);
    }
    q->first = q->first->next;
  }
  fclose(ansQuery3);
  closeHTMLTable(table);
}

static void freeRecYear(Tyear * l){
  if(l==NULL){
    return;
  }
  freeRecYear(l->next);
  free(l);
}

static void freeRecSen(TList l){
  if(l==NULL){
    return;
  }
  freeRecYear(l->tail);
  free(l);
}

void freeQuery(QueryADT q){
  freeRecSen(q->sensors);
  freeRecYear(q->first);
  for(int i = 0; i < DIM_SENS; i++){
    free(q->vecSen[i].name);
  }
  free(q->vecSen);
  free(q);
}
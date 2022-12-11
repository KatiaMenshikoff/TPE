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
    if (v[i].Namelen == 0) {
      continue;
    }
      q->vecSen[i].flag = v[i].flag;
      q->vecSen[i].Namelen = v[i].Namelen;
      q->vecSen[i].name = malloc(q->vecSen[i].Namelen + 1);
      if (q->vecSen[i].name == NULL) {
        perror("Unable to allocate memory.");
        exit(1);
      }
      strcpy(q->vecSen[i].name, v[i].name);
      q->vecSen[i].Tpedestrians = v[i].Tpedestrians;
  }
}

void insertList(QueryADT q, Tyear *l) { 
  q->first = l; 
}

TList addRec(TList list, size_t id, long int peds, char flag) { //NO ESTA COPIANDO
  if(flag == 'R' || flag==0){ // si el flag esta en 0, el espacio esta vacío.
    return list; //si el sensor este inactivo no quiero que aparezca en la fila.
  }
  if (list == NULL || peds > list->pedestrians) {
    TList aux = malloc(sizeof(TNode));
    if (aux == NULL) {
      perror("Not able to allocate memory.");
      exit(1);
    }
    aux->id = id;
    aux->pedestrians = peds;
    aux->tail = list;
    return aux;
  }
  list->tail = addRec(list->tail, id, peds, flag);
  return list;
}



void createList(QueryADT q, TSensor sensor[]) {
  int i;
  TList ans = NULL;
  for (i = 0; i < DIM_SENS; i++) {
    ans = addRec(ans, (i + 1), sensor[i].Tpedestrians, sensor[i].flag);  //NO GUARDA EN ANS
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
    char c[15];
    sprintf(c, "%li",q->sensors->pedestrians);
    addHTMLRow(table, q->vecSen[q->sensors->id - 1].name,c);
    q->sensors = q->sensors->tail;
  }
  closeHTMLTable(table);
  fclose(query1);
}

void query2(QueryADT q) {
  FILE *query2 = fopen("query2.csv", "wt");
  htmlTable table = newTable("query2.html", 4, "Year", "Weekdays Count",
                             "Weekends Count", "Total Count");
  fprintf(query2, "Year, Weekdays Count, Weekends Count, Total Count\n");
  while (q->first != NULL) {
    fprintf(query2, "%lu, %ld, %ld, %ld\n", q->first->year, q->first->Dweek,
            q->first->Dweekend, q->first->total);
    addHTMLRow(table, q->first->year, q->first->Dweek, q->first->Dweekend,
               q->first->total);
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
    if ((q->first->year) % 4 == 0) {
      fprintf(ansQuery3, "%lu, %.2f\n", q->first->year,
              (float)q->first->total / 366.0);
      addHTMLRow(table, q->first->year, (float)q->first->total / 366.0);
    } else {
      fprintf(ansQuery3, "%lu, %.2f\n", q->first->year,
              (float)q->first->total / 365.0);
      addHTMLRow(table, q->first->year, (float)q->first->total / 365.0);
    }
    q->first = q->first->next;
  }
  fclose(ansQuery3);
  closeHTMLTable(table);
}

static void freeRecYear(Tyear *l) {
  if (l == NULL) {
    return;
  }
  freeRecYear(l->next);
  free(l);
}

static void freeRecSen(TList l) {
  if (l == NULL) {
    return;
  }
  freeRecSen(l->tail);
  free(l);
}

void freeQuery(QueryADT q) {
  freeRecSen(q->sensors);
  freeRecYear(q->first);
  for (int i = 0; i < DIM_SENS; i++) {
    free(q->vecSen[i].name);
  }
  free(q->vecSen);
  free(q);
}

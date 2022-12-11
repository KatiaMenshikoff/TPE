#include "QueryTAD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#define MAX 15

typedef struct QueryCDT {
  TSensor vecSen[DIM_SENS];
  Tyear *first; // usamos lista para guardar los años en orden.
  TList sensors;
} QueryCDT;

QueryADT newQuery(void) {
  QueryADT new = calloc(1, sizeof(QueryCDT));
  if (new == NULL) {
    perror("Unable to allocate memory.");
    exit(1);
  }
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

void insertList(QueryADT q, Tyear *l) { q->first = l; }

static TList addRec(TList list, size_t id, long int peds,
                    TSensor sensors[]) { 
  if (sensors[id - 1].flag == 'R' ||
      sensors[id - 1].flag == 0) {   // si el flag esta en 0, el espacio esta vacío.
    return list; // si el sensor este inactivo no quiero que aparezca en la
                 // fila.
  }
  if (list == NULL || peds > list->pedestrians ||
      (peds == list->pedestrians &&
       strcasecmp(sensors[id - 1].name, sensors[list->id - 1].name) < 0)) {
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
  list->tail = addRec(list->tail, id, peds, sensors);
  return list;
}

void createList(QueryADT q, TSensor sensor[]) {
  int i;
  TList ans = NULL;
  for (i = 0; i < DIM_SENS; i++) {
    ans = addRec(ans, (i + 1), sensor[i].Tpedestrians,
                 sensor); 
  }
  if (ans == NULL) {
    perror("Unable to copy information.");
    exit(1);
  }
  q->sensors = ans;
}

void Query1(QueryADT q, FILE * query1, htmlTable table) {
  while (q->sensors != NULL) {
    fprintf(query1, "%s; %ld\n", q->vecSen[q->sensors->id - 1].name,
            q->sensors->pedestrians);
    char c[MAX];
    sprintf(c, "%li", q->sensors->pedestrians);
    addHTMLRow(table, q->vecSen[q->sensors->id - 1].name, c);
    q->sensors = q->sensors->tail;
  }
}

void Query2(QueryADT q, FILE * query2, htmlTable table2) {
  Tyear *aux = q->first;
  while (aux != NULL) {
    fprintf(query2, "%li; %li; %li; %li\n", aux->year, aux->Dweek,
            aux->Dweekend, aux->total);
    char a[MAX], b[MAX], c[MAX], d[MAX];
    sprintf(a, "%li", aux->year);
    sprintf(b, "%li", aux->Dweek);
    sprintf(c, "%li", aux->Dweekend);
    sprintf(d, "%li", aux->total);
    addHTMLRow(table2, a, b, c, d);
    aux = aux->next;
  }
}

void Query3(QueryADT q,FILE * query3, htmlTable table3) {
  Tyear *aux = q->first;
  while (aux != NULL) {
    if ((aux->year) % 4 == 0) {
      float i = (float)aux->total / 366.0;
      fprintf(query3, "%li; %.2f\n", aux->year, i);
      char y[MAX], a[MAX];
      sprintf(y, "%li", aux->year);
      sprintf(a, "%.2f", i);
      addHTMLRow(table3, y, a);
    } else {
      float j = (float)aux->total / 365.0;
      fprintf(query3, "%li; %.2f\n", aux->year, j);
      char y2[MAX], a2[MAX];
      sprintf(y2, "%li", aux->year);
      sprintf(a2, "%.2f", j);
      addHTMLRow(table3, y2, a2);
    }
    aux = aux->next;
  }
}

static void freeRecYear(TListYear l) {
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

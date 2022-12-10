#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "QueryTAD.h"

int main(void){
    /*query = newQuery;
    query->vecSen = makeVec;


    */
}



TSensor * makeVec(void){
    TSensor * ans;
    FILE * fSensor;
    fSensor = fopen("sensor.csv", "rt");
    char line[100];
    if (fSensor == NULL){
        perror("Unable to open the file.");
        exit(1);
    }
    while (!feof(fSensor)){
        for (int i=0; fgets(line, 100, fSensor); i++){
            char * value = strtok(line, "; ");
            while (value != NULL){
                size_t pos = atoi(value);
                value = strtok(NULL, "; ");
                ans[pos - 1].len = strlen(value);
                ans[pos - 1].name = malloc(ans[pos - 1].len + 1);  
                ans[pos - 1].name = strcpy(ans[pos - 1].name, value); 
                value = strtok(NULL, "; ");
                ans[pos - 1].flag = value;
                value = strtok(NULL, "; ");
            }
        }
    }
    fclose(fSensor);
    return ans;
}
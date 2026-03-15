#include "project.h"
#include <stdio.h>
#include <string.h>
/*
function to declare a struct for material types and their given density
*/
void computeForCeiling (Project s, int n){
 
    int i, j;

    for (i = 0; i < n; i++){
    /*
    to implement search algorithm for finding which material type is given
    
    */
    }
}

void compute_section_kgCO2e (Section *s, float constant){
    kgCO2e Amount;

    if (strcmp("Glass", s->resource.material) == 0){
        Amount = (s->volume) * (s->resource.embodiedCarbon);
    }
    else {
        Amount = constant * (s->volume) * (s->resource.embodiedCarbon);
    }

    s->value = Amount;  
}



void computeTotalSum (Section s){

}

void MaterialCatalogue (){    
}

void AssignCatalogue (Material *s,char *material, char *name, char *productname, char *materialtype, float embodiedcarbon){  
}

int linearSearch ()
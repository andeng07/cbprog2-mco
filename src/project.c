#include "project.h"
#include <stdio.h>
#include <string.h>
/*
function to declare a struct for material types and their given density
*/


void compute_section_kgCO2e (Section *sections, int constant){

    KgCO2ePerUnit carbon_value;
    KgCO2ePerUnit ceiling_value;
    float volume = sections->height * sections->width * sections->length;

    if (strcmp("Glass", sections->material.material.category) == 0){
        carbon_value = (volume) * (sections->material.embodied_carbon);
        ceiling_value = (volume) * (sections->material.ceiling);
    }
    else {
        carbon_value = constant * (volume) * (sections->material.embodied_carbon);
        ceiling_value = constant * (volume) * (sections->material.ceiling);
    }

    sections->carbon_value = carbon_value; 
    sections->ceiling_value = ceiling_value;
}

void compute_for_sum (Project *project, int array_size_n, KgCO2e *sum_project, KgCO2e *sum_ceiling){

        KgCO2e sum_1 = 0;
        KgCO2e sum_2 = 0;
        for (int i = 0; i < array_size_n; i++){
            sum_1 += project->sections[i].carbon_value;
            sum_2 += project->sections[i].ceiling_value;
    }

    *sum_project = sum_1;
    *sum_ceiling = sum_2;
}




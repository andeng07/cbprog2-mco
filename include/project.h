#ifndef PROJECT_H
#define PROJECT_H

#include "string_util.h"
#include "list_util.h"
#include <stdio.h>
#include <stdbool.h>

#define MATERIALS_FILE_NAME "materials.txt"
#define PROJECTS_FILE_NAME "projects.txt"

#define MAT_IN_FMT  " %u,%[^,],%[^,],%[^,],%[^,],%f,%f"
#define MAT_OUT_FMT "%d,%s,%s,%s,%s,%f,%f\n"

#define SEC_IN_FMT  " %[^,],%d,%f,%f,%f"
#define SEC_OUT_FMT "%s,%d,%f,%f,%f\n"

typedef float KgCO2e;
typedef float KgCO2ePerUnit;
typedef float DensityConstant;
typedef float Meter;

typedef struct {
    unsigned int id;
    String64 company_name;
    String64 category;
    String64 type;
    String64 product_name;
    KgCO2ePerUnit embodied_carbon;
    DensityConstant density;
} Material;

typedef List(Material) MaterialList;

typedef struct {
    String64 section_name;
    Material material;
    Meter length, width, height;
} Section;

typedef struct {
    unsigned int id;
    String64 project_name;
    KgCO2e total_carbon;
    bool is_approved;
    List(Section) sections;
} Project;

typedef List(Project) ProjectList;

// Material Operations
int read_material_element(FILE *file_ptr, Material *m);
void write_material_element(FILE *file_ptr, const Material *m);
void load_materials(MaterialList *list);
void save_materials(MaterialList *list);
int delete_material(MaterialList *list, unsigned int id);

int get_materials_by_category(MaterialList *dest, const MaterialList *source, char *category);

float calculate_embodied_carbon(Section *section);

Material *get_material_by_id(MaterialList *list, unsigned int id);

// Project Operations
int read_project_element(FILE *file_ptr, Project *p);
void write_project_element(FILE *file_ptr, const Project *p);
void load_projects(ProjectList *list, MaterialList *catalog);
void save_projects(ProjectList *list);
int delete_project(ProjectList *list, unsigned int id);

Project *get_project_by_id(ProjectList *list, unsigned int id);

float calculate_project_ee(Project *project);
float calculate_project_ee_ceiling(MaterialList *material_list, Project *project);

#endif

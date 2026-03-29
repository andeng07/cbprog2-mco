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

/**
 * unit aliases
 */
typedef float KgCO2e;
typedef float KgCO2ePerUnit;
typedef float DensityConstant;
typedef float Meter;

/**
 * represents a raw construction material and its environmental properties.
 */
typedef struct {
    unsigned int id;
    String64 company_name;
    String64 category;
    String64 type;
    String64 product_name;
    KgCO2ePerUnit embodied_carbon;
    DensityConstant density;
} Material;

/**
 * type definition for a list of material structures.
 */
typedef List(Material) MaterialList;

/**
 * defines a specific component of a project using a material and dimensions.
 */
typedef struct {
    String64 section_name;
    Material material;
    Meter length, width, height;
} Section;

/**
 * represents a complete building or structure with carbon analysis data.
 */
typedef struct {
    unsigned int id;
    String64 project_name;
    List(Section) sections;
} Project;

/**
 * type definition for a list of project structures.
 */
typedef List(Project) ProjectList;

// material operations

/**
 * reads a single material from a file.
 * @param file_ptr pointer to the source file.
 * @param m pointer to destination material struct.
 * @return 1 if successful, 0 otherwise.
 */
int read_material_element(FILE *file_ptr, Material *m);

/**
 * writes a single material to a file.
 * @param file_ptr pointer to the destination file.
 * @param m pointer to source material struct.
 */
void write_material_element(FILE *file_ptr, const Material *m);

/**
 * loads all materials from the default catalog file.
 * @param list pointer to material list to populate.
 */
void load_materials(MaterialList *list);

/**
 * saves the material list to the default catalog file.
 * @param list pointer to material list to save.
 */
void save_materials(MaterialList *list);

/**
 * deletes a material by id and updates the file.
 * @param list pointer to the material list.
 * @param id the id of the material to remove.
 * @return 1 if removed, 0 if not found.
 */
int delete_material(MaterialList *list, unsigned int id);

/**
 * filters the material list by a specific category.
 * @param dest list to store matching materials.
 * @param source catalog list to search through.
 * @param category string representing the category to match.
 * @return 1 if successful.
 */
int get_materials_by_category(MaterialList *dest, const MaterialList *source, char *category);

/**
 * calculates carbon footprint for a specific section based on volume and density.
 * @param section pointer to the section to calculate.
 * @return the calculated kgco2e.
 */
float calculate_embodied_carbon(Section *section);

/**
 * finds a material in the list by its id.
 * @param list pointer to the material list.
 * @param id the id to find.
 * @return pointer to the found material, or null.
 */
Material *get_material_by_id(MaterialList *list, unsigned int id);

// project operations

/**
 * reads a project and its nested sections from a file.
 * @param file_ptr pointer to the source file.
 * @param p pointer to destination project struct.
 * @return 1 if successful.
 */
int read_project_element(FILE *file_ptr, Project *p);

/**
 * writes a project and all its sections to a file.
 * @param file_ptr pointer to destination file.
 * @param p pointer to source project.
 */
void write_project_element(FILE *file_ptr, const Project *p);

/**
 * loads projects from file and links section materials to the catalog.
 * @param list pointer to project list.
 * @param catalog loaded material catalog for lookups.
 */
void load_projects(ProjectList *list, MaterialList *catalog);

/**
 * saves the project list to the default file.
 * @param list pointer to project list.
 */
void save_projects(ProjectList *list);

/**
 * deletes a project and frees its internal sections list.
 * @param list pointer to the project list.
 * @param id id of project to remove.
 * @return 0 after completion.
 */
int delete_project(ProjectList *list, unsigned int id);

/**
 * finds a project in the list by its id.
 * @param list pointer to the project list.
 * @param id the id to find.
 * @return pointer to the found project, or null.
 */
Project *get_project_by_id(ProjectList *list, unsigned int id);

/**
 * calculates the sum of carbon emissions for all sections in a project.
 * @param project pointer to the project.
 * @return total embodied carbon.
 */
float calculate_project_ee(Project *project);

/**
 * calculates a theoretical carbon ceiling (60% of the emissions if using the worst materials in each category).
 * @param material_list catalog of all materials.
 * @param project pointer to the project.
 * @return the calculated ceiling value.
 */
float calculate_project_ee_ceiling(MaterialList *material_list, Project *project);

#endif

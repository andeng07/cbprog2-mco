#ifndef PROJECT_H
#define PROJECT_H
#define CEMENT_DENSITY_CONSTANT 2400
#define STEEL_DENSITY_CONSTANT 7840
#define WOOD_DENSITY_CONSTANT 644
#include "string_util.h"
#include <stdbool.h>

typedef float KgCO2e;
typedef float KgCO2ePerUnit;
typedef float Constant;
typedef float Meter;
typedef float CubicMeter;

typedef struct {
    String8  category; //{"Cement", "Glass", "Steel", "Wood"}
    Constant density; //{CEMENT_DENSITY_CONSTANT, NULL, STEEL_DENSITY_CONSTANT, WOOD_DENSITY_CONSTANT}
} Category;

typedef struct {
    String64 company_name;
    String64 product_name;

    Category material;   // wood, steel, glass, cement
    String64 material_type;

    KgCO2ePerUnit embodied_carbon;

} Material;

typedef struct {
    String64 section_name;

    /* Associated material */
    Material material;

    /* Dimensions */
    Meter height;
    Meter width;
    Meter length;

    /* Carbon calculation */
    KgCO2e carbon_value;
} Section;

typedef struct {
    String64 project_name;

    KgCO2e total_carbon;
    bool is_approved;

    Section sections[50];

} Project;

typedef struct {
    String64 company_name;

    Project projects[100];

} Company;

void compute_for_ceiling(Project s, int n);
void compute_section_kgCO2e(Section *s, float constant);
void compute_total_sum(Section s);

#endif

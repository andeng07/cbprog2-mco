#ifndef PROJECT_H
#define PROJECT_H
#define CEMENT_DENSITY_CONSTANT 2400
#define STEEL_DENSITY_CONSTANT 7840
#include "string_util.h"
#include <stdbool.h>

typedef float KgCO2e;
typedef float KgCO2ePerCubicMeter;
typedef float Meter;
typedef float CubicMeter;

typedef struct {
    String64 company;
    String64 product;

    String8  materialCategory;   // wood, steel, glass, cement
    String64 material;

    KgCO2ePerCubicMeter embodiedCarbon;

} Material;

typedef struct {
    String64 sectionName;

    /* Dimensions */
    Meter height;
    Meter width;
    Meter length;

    CubicMeter volume;

    /* Carbon calculation */
    KgCO2e carbonValue;

    /* Associated material */
    Material material;

} Section;

typedef struct {
    String64 projectName;

    KgCO2e totalCarbon;
    bool isApproved;

    Section sections[50];

} Project;

typedef struct {
    String64 constructionCompany;

    Project projects[100];

} Company;

#endif 
#ifndef PROJECT_H
#define PROJECT_H
#define CEMENT_DENSITY_CONSTANT 2400
#define STEEL_DENSITY_CONSTANT 7840

typedef float kgCO2e;
typedef float kgCO2e_CubicMeters;
typedef float meters;
typedef float Cubic_Meters;
typedef char Type [21];
typedef char Name [101];

typedef struct{
    Name company;
    Name product;
    Type material;
    kgCO2e_CubicMeters embodiedCarbon;
} Material;

typedef struct {
    Name constructionCompany[101];
    Project approved[100];
    Project rejected[100];
} Company;

typedef struct {
    Name project;
    kgCO2e total;
    Section part[];

} Project;

typedef struct {
    Name section;
    meters height;
    meters width;
    meters length;
    Cubic_Meters volume;
    kgCO2e value;
    Material resource;

} Section;

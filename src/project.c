#include "project.h"
#include "file_util.h"
#include "list_util.h"
#include <stdio.h>
#include <stdlib.h>

int read_material_element(FILE *fp, Material *m) {
  if (!fp || !m)
    return 0;
  return fscanf(fp, MAT_IN_FMT, &m->id, m->company_name, m->category, m->type,
                m->product_name, &m->embodied_carbon, &m->density) == 7;
}

void write_material_element(FILE *fp, const Material *m) {
  if (!fp || !m)
    return;
  fprintf(fp, MAT_OUT_FMT, m->id, m->company_name, m->category, m->type,
          m->product_name, m->embodied_carbon, m->density);
}

void load_materials(MaterialList *list) {
  load_list_from_file(MATERIALS_FILE_NAME, list, read_material_element);
}

void save_materials(MaterialList *list) {
  write_list_to_file(MATERIALS_FILE_NAME, list, write_material_element);
}

int is_material_id_equal(Material *a, Material *b) { return a->id == b->id; }

Material *get_material_by_id(MaterialList *list, unsigned int id) {
  Material *result = NULL;
  Material search = {.id = id};

  list_find_first(list, search, is_material_id_equal, &result);

  return result;
}

int delete_material(MaterialList *list, unsigned int id) {
  if (!list)
    return 0;

  int removed = 0;

  for (int i = 0; i < list->size && !removed; i++)
    if (list->items[i].id == id) {
      list_remove_at(list, i);
      removed = 1;
    }

  save_materials(list);

  return removed;
}

int is_category_equal(Material *a, Material *b) {
  return strcmp(a->category, b->category) == 0;
}

int get_materials_by_category(MaterialList *dest, const MaterialList *source,
                              char *category) {
  if (!dest || !source)
    return 0;

  Material search_mat;
  strcpy(search_mat.category, category);

  list_filter(dest, source, search_mat, is_category_equal);

  return 1;
}

float calculate_embodied_carbon(Section *section) {
  float result = 0;

  result = section->length * section->width * section->height *
           section->material.density * section->material.embodied_carbon;

  return result;
}

int read_project_element(FILE *fp, Project *p) {
  if (!fp || !p)
    return 0;

  if (fscanf(fp, " %d,%[^,\n]", &p->id, p->project_name) != 2) {
    return 0;
  }

  int num_sections = 0;
  if (fscanf(fp, " %d", &num_sections) != 1) {
    return 0;
  }

  p->sections.items = malloc(sizeof(Section) * num_sections);
  p->sections.size = 0;
  p->sections.cap = num_sections;

  for (int i = 0; i < num_sections; i++) {
    Section s = {0};
    int mat_id = 0;

    if (fscanf(fp, SEC_IN_FMT, s.section_name, &mat_id, &s.length, &s.width,
               &s.height) == 5) {
      s.material.id = mat_id;
      p->sections.items[p->sections.size++] = s;
    }
  }

  return 1;
}

void write_project_element(FILE *fp, const Project *p) {
  if (!fp || !p)
    return;

  fprintf(fp, "%d,%s\n%d\n", p->id, p->project_name, p->sections.size);

  for (int i = 0; i < p->sections.size; i++) {
    Section *s = &p->sections.items[i];
    fprintf(fp, SEC_OUT_FMT, s->section_name, s->material.id, s->length,
            s->width, s->height);
  }
}

void load_projects(ProjectList *list, MaterialList *catalog) {
  load_list_from_file(PROJECTS_FILE_NAME, list, read_project_element);

  for (int i = 0; i < list->size; i++) {
    Project *p = &list->items[i];

    for (int j = 0; j < p->sections.size; j++) {
      Section *sec = &p->sections.items[j];
      Material *found_mat = NULL;
      Material search_ref = {.id = sec->material.id};

      list_find_first(catalog, search_ref, is_material_id_equal, &found_mat);

      if (found_mat) {
        sec->material = *found_mat;
      }
    }
  }
}

void save_projects(ProjectList *list) {
  write_list_to_file(PROJECTS_FILE_NAME, list, write_project_element);
}

int is_project_id_equal(Project *a, Project *b) { return a->id == b->id; }

Project *get_project_by_id(ProjectList *list, unsigned int id) {
  Project *result = NULL;
  Project search = {.id = id};

  list_find_first(list, search, is_project_id_equal, &result);

  return result;
}

int delete_project(ProjectList *list, unsigned int id) {
  if (!list)
    return 0;

  for (int i = 0; i < list->size; i++) {
    if (list->items[i].id == id) {
      if (list->items[i].sections.items) {
        free(list->items[i].sections.items);
      }

      list_remove_at(list, i);
    }
  }

  save_projects(list);

  return 0;
}

float calculate_project_ee(Project *project) {
  float ee_sum = 0;

  list_foreach(Section, sec, &project->sections) {
    ee_sum += calculate_embodied_carbon(sec);
  }

  return ee_sum;
}

int mat_cmp(Material *a, Material *b) {
  if (a->embodied_carbon > b->embodied_carbon)
    return -1;
  else if (a->embodied_carbon < b->embodied_carbon)
    return 1;
  else
    return 0;
}

float calculate_project_ee_ceiling(MaterialList *material_list,
                                   Project *project) {
  float max_embodied_emission_sum = 0;

  list_foreach(Section, sec, &project->sections) {
    MaterialList filtered = EMPTY_LIST;

    get_materials_by_category(&filtered, material_list, sec->material.category);
    list_sort(&filtered, mat_cmp);

    max_embodied_emission_sum += sec->length * sec->width * sec->height *
                                 sec->material.density *
                                 list_first(&filtered)->embodied_carbon;
  }

  return max_embodied_emission_sum * 0.6;
}

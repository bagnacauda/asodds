#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include <curl/curl.h>
#include "myhtml/api.h"
#include <getopt.h>

#define MATRIX_ROWS 100
#define MATRIX_INFO 11

typedef struct asodds* asodds_t;

typedef struct res_html* res_html_t;

void credits();

void print_tree(myhtml_tree_t* tree, myhtml_tree_node_t* node);

void print_matrix(char* matches[MATRIX_ROWS][MATRIX_INFO]);

void print_array(int len, char* matches[MATRIX_INFO]);

void fill_matrix(char* matches[], myhtml_tree_t* tree, myhtml_tree_node_t* node, int i);

void clean_matrix(char* matrix[MATRIX_ROWS][MATRIX_INFO]);

void free_matrix(char* matrix[MATRIX_ROWS][MATRIX_INFO]);

void get_drops(char* matrix[MATRIX_ROWS][MATRIX_INFO]);

void asodds_parse(asodds_t mode);

void download(FILE* fp);

res_html_t load_html(char* file);

asodds_t asodds_init(int argc, char** argv);

char* asodds_file(asodds_t);

void asodds_run(asodds_t asodds, char* matches[MATRIX_ROWS][MATRIX_INFO]);

void asodds_clean(asodds_t);

#endif // HELPER_H_INCLUDED

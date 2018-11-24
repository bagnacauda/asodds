#ifndef HELPER_H_INCLUDED
#define HELPER_H_INCLUDED

#include <curl/curl.h>
#include "myhtml/api.h"
#include <getopt.h>

#define OFFSET 15
#define MATRIX_ROWS 100
#define MATRIX_INFO 11
#define FILENAME "/tmp/asianodds.html"
#define CREDITS "\tAsianodds parser version 0.1 alpha\n\tCredits: SimonDottor (code)\n"
#define H_ARR_LEN 11
#define A_ARR_LEN 8
#define LINE printf("\n")
#define CELL_SIZE 50

enum HOME_ARR { TIME, H_TEAM, H_SPREAD_NOW, H_ODD_NOW, H_SPREAD_OLD, H_ODD_OLD, TOT_NOW, TOT_OLD, H_TEXT_OVR, ODD_OVR_NOW, ODD_OVR_OLD};
enum AWAY_ARR { A_TEAM, A_SPREAD_NOW, A_ODD_NOW, A_SPREAD_OLD, A_ODD_OLD, A_TEXT_UND, ODD_UND_NOW, ODD_UND_OLD};

typedef struct asodds* asodds_t;

typedef struct res_html
{
    char* html;
    size_t size;
} res_html;

void credits();

void print_tree(myhtml_tree_t* tree, myhtml_tree_node_t* node);

void print_matrix(char* matches[MATRIX_ROWS][MATRIX_INFO]);

void print_array(int len, char* matches[MATRIX_INFO]);

void fill_matrix(char* matches[], myhtml_tree_t* tree, myhtml_tree_node_t* node, int i);

void clean_matrix(char* matrix[MATRIX_ROWS][MATRIX_INFO]);

void free_matrix(char* matrix[MATRIX_ROWS][MATRIX_INFO]);

void get_drops(char* matrix[MATRIX_ROWS][MATRIX_INFO]);

void parse(myhtml_tree_t* tree, asodds_t mode);

void download(FILE* fp);

res_html load_html(char* file);

asodds_t asodds_init(int argc, char** argv);

char* asodds_file(asodds_t);

void asodds_clean(asodds_t);

void asodds_run(asodds_t asodds, char* matches[MATRIX_ROWS][MATRIX_INFO]);

#endif // HELPER_H_INCLUDED

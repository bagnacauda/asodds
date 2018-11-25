#include "../lib/helper.h"

#define MAX_FILE 50
#define FILENAME "/tmp/asianodds.html"
#define CREDITS "\tAsianodds parser version 0.1 alpha\n\tCredits: SimonDottor (code)\n"
#define H_ARR_LEN 11
#define A_ARR_LEN 8
#define LINE printf("\n")
#define CELL_SIZE 50
#define OFFSET 15

enum HOME_ARR { TIME, H_TEAM, H_SPREAD_NOW, H_ODD_NOW, H_SPREAD_OLD, H_ODD_OLD, TOT_NOW, TOT_OLD, H_TEXT_OVR, ODD_OVR_NOW, ODD_OVR_OLD};
enum AWAY_ARR { A_TEAM, A_SPREAD_NOW, A_ODD_NOW, A_SPREAD_OLD, A_ODD_OLD, A_TEXT_UND, ODD_UND_NOW, ODD_UND_OLD};


typedef void (*a_mode)(char* matrix[MATRIX_ROWS][MATRIX_INFO]);

struct asodds
{
    a_mode mode;
    char* file;
};

struct res_html
{
    char* html;
    size_t size;
};


void credits()
{
    printf(CREDITS);
}

void print_tree(myhtml_tree_t* tree, myhtml_tree_node_t* node)
{
    while(node)
    {
        if (myhtml_node_tag_id(node) == MyHTML_TAG__TEXT)
        {
            const char* text = myhtml_node_text(node, NULL);
            printf("%s ", text);
        }
        node = myhtml_node_child(node);
    }
}

void print_matrix(char* matches[MATRIX_ROWS][MATRIX_INFO])
{
    for(int i=0; i < MATRIX_ROWS; i++)
    {
        for(int j=0; j < (i%2?A_ARR_LEN:H_ARR_LEN); j++)
        {
            printf("%s\t", matches[i][j]);
        }
        LINE;
        if(i%2 == 1)
            LINE;
    }
}

void print_array(int len, char* matches[MATRIX_INFO])
{
    for(int i=0; i < len; i++)
    {
        printf("%s\t", matches[i]);
    }
    LINE;
}

void fill_matrix(char* matches[], myhtml_tree_t* tree, myhtml_tree_node_t* node, int i)
{
    while(node)
    {
        int id = myhtml_node_tag_id(node);
        if (id == MyHTML_TAG__TEXT)
        {
            char* temp = (char*) malloc(sizeof(char)*CELL_SIZE);
            const char* text = myhtml_node_text(node, NULL);
            if(text)
            {
                strcpy(temp, text);
                matches[i] = temp;
            }

        }
        node = myhtml_node_child(node);
    }
}

void clean_matrix(char* matrix[MATRIX_ROWS][MATRIX_INFO])
{
    for(int i = 0; i < MATRIX_ROWS; i++ )
    {
        for(int j=0; j< MATRIX_INFO; j++)
        {
            matrix[i][j] = NULL;
        }
    }
}

void free_matrix(char* matrix[MATRIX_ROWS][MATRIX_INFO])
{
    for(int i = 0; i < MATRIX_ROWS; i++ )
    {
        for(int j=0; j< MATRIX_INFO; j++)
        {
            free(matrix[i][j]);
        }
    }
}

void get_drops(char* matrix[MATRIX_ROWS][MATRIX_INFO])
{
    for(int i=0; i < MATRIX_ROWS; i+=2)
    {
        if(atof(matrix[i][TOT_NOW]) < atof(matrix[i][TOT_OLD]))
        {
            print_array(H_ARR_LEN, matrix[i]);
            print_array(A_ARR_LEN, matrix[i+1]);
            LINE;
        }
    }
}

void asodds_parse(asodds_t asodds)
{
    char* file = asodds_file(asodds);
    res_html_t res = load_html(file);

    //MyHTML init
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    //init tree
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);

    //parse html
    myhtml_parse(tree, MyENCODING_UTF_8, res->html, res->size);

    myhtml_collection_t *collection = myhtml_get_nodes_by_attribute_value(tree, NULL, NULL, false, "class", strlen("class"), "main", strlen("main"), NULL);
    myhtml_collection_t *tds;
    if(collection && collection->list && collection->length)
    {
        char* matches[MATRIX_ROWS][MATRIX_INFO];
        clean_matrix(matches);
        for(int i=OFFSET; i< collection->length; i++)
        {
            tds = myhtml_get_nodes_by_tag_id_in_scope(tree, NULL, collection->list[i], MyHTML_TAG_TD, NULL);
            for(int j=0; j< tds->length; j++)
            {
                fill_matrix(matches[i-OFFSET], tree, tds->list[j], j);
            }
        }
        res_html_clean(res);
        asodds_run(asodds, matches);
        free_matrix(matches);
    }
}

void download(FILE* fp)
{
    CURL* curl = curl_easy_init();
    if(curl)
    {
        char error[CURL_ERROR_SIZE];
        curl_easy_setopt(curl, CURLOPT_FILE, fp);
        curl_easy_setopt(curl, CURLOPT_URL, "asianodds.com");
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, error);
        CURLcode code = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if(code != CURLE_OK)
        {
            printf("%s\n", error);
            exit(-1);
        }
    }

}

res_html_t load_html(char* file)
{
    FILE* fh = NULL;
    if(file)
    {
        printf("%s\n", file);
        fh = fopen(file, "r");
    }
    else
    {
        file = FILENAME;
        fh = fopen(file, "w+");
        download(fh);
    }

    if(fh == NULL)
    {
        fprintf(stderr, "Can't open html file: %s\n", file);
        exit(-1);
    }

    if(fseek(fh, 0L, SEEK_END) != 0)
    {
        fprintf(stderr, "Can't set the position in file: %s", file);
        exit(-1);
    }

    long size = ftell(fh);

    if(fseek(fh, 0L, SEEK_SET) != 0)
    {
        fprintf(stderr, "Can't set the position in file: %s", file);
        exit(-2);
    }

    if(size <=0)
    {
        fprintf(stderr, "Empty file: %s\n", file);
        exit(-3);
    }

    char *html = (char*) malloc(size +1);

    if(html == NULL)
    {
        fprintf(stderr, "Not able to allocate memory");
        exit(-4);
    }

    size_t nread = fread(html, 1, size, fh);

    if(nread != size)
    {
        exit(-5);
    }

    res_html_t ret = malloc(sizeof(struct res_html));

    ret->html = html;
    ret->size = size;

    fclose(fh);
    return ret;
}

asodds_t asodds_init(int argc, char** argv)
{
    asodds_t mode = malloc(sizeof(struct asodds));

    if(argc < 2)
    {
        printf("USAGE: %s mode", argv[0]);
        exit(-1);
    }

    while(1)
    {

        static struct option long_options[] =
        {
            {"drops",     no_argument,    0, 'd'},
            {"help", no_argument, 0, 'h'},
            {"offline", required_argument, 0, 'o'},
            {"print",     no_argument,    0, 'p'},
            {"version", no_argument, 0, 'v'},
            {0, 0, 0, 0}
        };

        int opt_index = 0;
        char c = getopt_long(argc, argv, "dho:pv", long_options, &opt_index);
        if(c == -1)
        {
            break;
        }
        switch(c)
        {
            char* temp;
        case 'd':
            mode->mode = &get_drops;
            break;
        case 'h':
            printf("====ASIANODDS PARSER====\n\nUsage: %s mode\n\nModes\n* --print:\tprints all matches found\n* --drops:\tprints only matches with a drop in spread\n", argv[0]);
            exit(0);
        case 'o':
            mode->file = malloc(sizeof(char)*strlen(optarg));
            strcpy(mode->file, optarg);
            break;
        case 'p':
            mode->mode = &print_matrix;
            break;
        case 'v':
            printf(CREDITS);
            exit(0);
            break;
        case '?':
            //No parameter
            printf("No argument\n");
            exit(-1);
        default:
            printf("Option not valid. Try --help for more info.\nAborting...\n");
            exit(-1);
        }
    }

    if(!mode->mode)
    {
        printf("No mode set. Aborting...\n");
        exit(-1);
    }
    return mode;
}

void res_html_clean(res_html_t res){
    free(res->html);
    free(res);
}

char* asodds_file(asodds_t asodds){
    return asodds->file;
}

void asodds_clean(asodds_t asodds){
    free(asodds->file);
    free(asodds);
}

void asodds_run(asodds_t asodds, char* matches[MATRIX_ROWS][MATRIX_INFO]){
    asodds->mode(matches);
}

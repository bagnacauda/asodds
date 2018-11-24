#include "helper.h"

int main(int argc, char** argv)
{
    asodds_t asodds;
    asodds = asodds_init(argc, argv);
    char* file = asodds_file(asodds);
    res_html res = load_html(file);


    //MyHTML init
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);

    //init tree
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);

    //parse html
    myhtml_parse(tree, MyENCODING_UTF_8, res.html, res.size);

    //parse matches
    parse(tree, asodds);

    //cleaning up mess
    asodds_clean(asodds);
    return 0;
}

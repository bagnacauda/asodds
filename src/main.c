#include "../lib/helper.h"

int main(int argc, char** argv)
{
    asodds_t asodds;
    asodds = asodds_init(argc, argv);

    //parse matches
    asodds_parse(asodds);

    //cleaning up mess
    asodds_clean(asodds);
    return 0;
}

#include "../lib/helper.h"

int main(int argc, char** argv)
{
    asodds_t asodds;
    asodds = asodds_init(argc, argv);

    asodds_parse(asodds);

    asodds_clean(asodds);
    return 0;
}

#include "pre_assembler.h"

int main(int argc, char *argv[]) {
    if (!deploy_macros(argc, argv)) return 0;

    return 1;
}

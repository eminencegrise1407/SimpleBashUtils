#include "options.h"

int main(int argc, char **argv) {
    options Options = {0};
    parser(argc, argv, &Options);
    char *pattern = argv[optind];
    while (optind < argc) {
        command_line_out(pattern, Options, argv[optind]);
        optind++;
    }
    return 0;
}

#ifndef SRC_OPTIONS_H
#define SRC_OPTIONS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <getopt.h>
#include <regex.h>

typedef struct options {
    int e;  // Pattern search
    int i;  // search ignoring the case
    int v;  // select non-matching lines
    int c;  // count matches
    int l;  // name of the file with a match
    int n;  // number of the line with a match
    int h;  // no filename when got a match
    int s;  // no error messages about nonexistent and unreadable files
    int f;  // obtain patterns from FILE, one per line
    int o;  // print only matched parts
} options;

int parser(int argc, char **argv, options *Options);
int command_line_out(char *pattern, options Options, char *f);

#endif // SRC_OPTIONS_H

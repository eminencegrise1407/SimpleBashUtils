#include "options.h"
// flag -c works twice
// add -o -f
int parser(int argc, char **argv, options *Options) {
    int res = 0;
    int flag = 1;

    while ((res = getopt(argc, argv, "eivclnhsfo")) != -1) {
        switch (res) {
		case 'e':
			Options->e = 1;
			break;
		case 'i':
			Options->i = 1;
			break;
		case 'v':
			Options->v = 1;
			break;
		case 'c':
			Options->c = 1;
			break;
		case 'l':
			Options->l = 1;
			break;
		case 'n':
			Options->n = 1;
			break;
        case 'h':
            Options->h = 1;
            break;
        case 's':
            Options->s = 1;
            break;
        case 'f':
            Options->f = 1;
            break;
        case 'o':
            Options->o = 1;
            break;        
        case '?':
        default:
            flag = res;
            break;
        }
    }
//   if (needOpt == 1) {
//     Options->e = 1;
//     Options->noOpt = 1;
//   }

    if (optind + 2 < argc) {
        Options->notOneFile = 1;
    }
    return flag;    
}

int command_line_out(char *pattern, options Options, char *filename) {
    //int flag = 1;
    FILE *fp;
    size_t n;
    char *buffer = NULL;
    int counter = 0;
    int string_counter = 0;
    regex_t reg;
    regcomp(&reg, pattern, REG_EXTENDED);
    int search;

    if ((fp = fopen(filename, "r")) != NULL) {
        // check if file is a directory
        // TODO: add option to Options sctruct to check if i need a filename
        // add those to output
        // flags -i -l -hsfo
        while (getline (&buffer, &n, fp) != -1) {
            string_counter++;
            search = regexec(&reg, buffer, 0, NULL, 0);
        if (Options.i) {
        	regfree(&reg);
		    regcomp(&reg, pattern, REG_ICASE);
		    search = regexec(&reg, buffer, 0, NULL, 0);
	    }
            if (Options.v) {
                search = !search;
            }
        if (Options.h) {
                Options.notOneFile = 0;
        }
            if (search == 0) {
                counter++;
                if (buffer[strlen(buffer) - 1] != '\n') {
                    int end = strlen(buffer);
                    buffer[end] = '\n';
                    buffer[end + 1] = '\0';
                }
                if (Options.c != 1 && Options.l != 1) {
                    if (Options.notOneFile && Options.n) {
                        printf("%s:%d:%s", filename, string_counter, buffer);
                    } else if (Options.notOneFile && !Options.n) {
                        printf("%s:%s", filename, buffer);
                    } else if (!Options.notOneFile && Options.n) {
                        printf("%d:%s", string_counter, buffer);
                    } else {
                        printf("%s", buffer);
                    }
                }
            }
        }
        if (Options.c && !Options.l) {
            if (Options.notOneFile) {
                printf("%s:%d\n", filename, counter);
            } else {
                printf("%d\n", counter);
            }
        }

        if (Options.c && !Options.l) {
            if (Options.notOneFile) {
                printf("%s:%d\n", filename, counter);
            } else {
                printf("%d\n", counter);
            }
        }

        if (Options.c && Options.l) {
            if (counter == 0) {
                counter = 0;
            } else {
                counter = 1;
            }
            if (Options.notOneFile) {
                printf("%s:%d\n", filename, counter);
            } else {
                printf("%d\n", counter);
            }
        }

        if (Options.l && counter != 0) {
            counter = 1;
            printf("%s\n", filename);
        }

        fclose(fp);
    }
    regfree(&reg);
    return 1;
}

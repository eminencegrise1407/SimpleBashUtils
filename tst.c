#include "s21_grep.h"

void gimmi_some_action(char *str, char **argv, struct Options *option,
                       char *search_string, FILE *file, int *l_count,
                       int *v_count, int files_count) {
  regex_t regex;
  int err;
  int success = 0;
  int count = 0;
  int n_count = 0;
  if (option->iflag) {
    err = regcomp(&regex, search_string, REG_ICASE);
  } else {
    err = regcomp(&regex, search_string, REG_EXTENDED);
  }
  if (err != 0) {
    if (option->sflag) {
      exit(1);
    } else {
      printf("ERROR\n");
      exit(1);
    }
  }
  while (!feof(file)) {
    if (fgets(str, 1024, file)) {
      *v_count += 1;
      count++;
      success = regexec(&regex, str, 0, NULL, 0);
      if (success == 0) {
        success_regex(argv, option, str, l_count, files_count, count, &n_count,
                      search_string);
        if ((str[(strlen(str)) - 1] != '\n') && !option->oflag &&
            !option->cflag && !option->lflag && !option->vflag) {
          printf("\n");
        }
      } else if (option->vflag && !option->cflag && !option->lflag) {
        if (files_count > 1) {
          printf("%s:", argv[optind]);
        } else if (option->nflag) {
          printf("%d:", *v_count);
        }
        printf("%s", str);
        if ((str[(strlen(str)) - 1] != '\n') && option->vflag) {
          printf("\n");
        }
      }
    }
  }
  if (option->cflag) {
    c_flag(argv, option, v_count, l_count, files_count);
  }
  if (option->lflag && *l_count) {
    printf("%s\n", argv[optind]);
  }
  regfree(&regex);
}

void chosen_flag(int argc, char **argv, struct Options *option, char *patt_e,
                 char *patt_f) {
  int f;
  while ((f = getopt(argc, argv, "e:ivclnhsof:")) != -1) {
    switch (f) {
    case 'e':
      option->eflag += 1;
      strcat(patt_e, optarg);
      strcat(patt_e, "|");
      break;
    case 'i':
      option->iflag += 1;
      break;
    case 'v':
      option->vflag += 1;
      break;
    case 'c':
      option->cflag += 1;
      break;
    case 'l':
      option->lflag += 1;
      break;
    case 'n':
      option->nflag += 1;
      break;
    case 'h':
      option->hflag += 1;
      break;
    case 's':
      option->sflag += 1;
      break;
    case 'f':
      option->fflag += 1;
      read_fflag(option, patt_f);
      break;
    case 'o':
      option->oflag += 1;
      break;
    default:
    case '?':
      printf("FAILURE");
    }
  }
}

void read_fflag(struct Options *option, char *patt_f) {
  char str_2[100];
  FILE *new = fopen(optarg, "r");
  if (!new) {
    if (!option->sflag) {
      printf("Can not open file");
    }
    exit(1);
  } else {
    while (!feof(new)) {
      if (fgets(str_2, 1024, new)) {
        strcat(patt_f, str_2);
        strcat(patt_f, "|");
      }
    }
  }
  fclose(new);
}

void c_flag(char **argv, struct Options *option, int *v_count, int *l_count,
            int files_count) {
  if (option->lflag && *l_count) {
    printf("1\n");
  } else if (files_count && !option->hflag) {
    if (files_count > 1) {
      printf("%s:", argv[optind]);
    }
    if (option->vflag) {
      printf("%d\n", ((*v_count) - (*l_count)));
    } else {
      printf("%d\n", *l_count);
    }
  } else {
    printf("%d\n", *l_count);
  }
}

void success_regex(char **argv, struct Options *option, char *str, int *l_count,
                   int files_count, int count, int *n_count,
                   char *search_string) {
  int flag = 0;
  *l_count += 1;
  n_count++;
  if (!option->cflag && !option->lflag && !option->vflag) {
    if (files_count > 1 && !option->hflag) {
      printf("%s:", argv[optind]);
    }
    if (option->nflag) {
      printf("%d:", count);
    }
    if (option->oflag && !option->vflag) {
      const char *sp = str;
      while ((sp = strstr(sp, search_string)) != NULL) {
        printf("%s\n", search_string);
        sp += strlen(search_string);
        flag = 1;
      }
    }
    if (!flag) {
      printf("%s", str);
    }
  }
}

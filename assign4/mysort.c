#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "samples/prototypes.h"

#define MAX_LINE_LEN 4096
#define MIN_NLINES 100

typedef int (*cmp_fn_t)(const void *p, const void *q);
typedef int (*print_function_t)(char **, size_t);

int cmp_pstr(const void *p, const void *q) {
    // TODO(you!): implement this function (and remove the line below)
    return strcmp(*(char **)p, *(char **)q);
}

int cmp_pstr_len(const void *p, const void *q) {
    // TODO(you!): implement this function (and remove the line below)
    char *p_string = *(char **)p;
    char *q_string = *(char **)q;
    return strlen(p_string) - strlen(q_string);
}

int cmp_pstr_numeric(const void *p, const void *q) {
    // TODO(you!): implement this function (and remove the line below)
    char *p_string = *(char **)p;
    char *q_string = *(char **)q;
    return atoi(p_string) - atoi(q_string);
}

void print_and_free(char **arr, size_t count, bool reverse) {
    if (reverse) {
        for (int i = count - 1; i >= 0; i--) {
            printf("%s", arr[i]);
            free(arr[i]);
        }
    }
    else {
        for (int i = 0; i < count; i++) {
            printf("%s", arr[i]);
            free(arr[i]);        
        }
    }
    free(arr);
}
/* This function is a simplified version of the Unix sort command.
 * It takes in the name of a file and print its lines in sorted order.
 * If parameter uniq is on, the function will discard duplicate lines.
 * If parameter reverse is on, the function will print the line in reverse order.
 */
void sort_lines(FILE *fp, cmp_fn_t cmp, bool uniq, bool reverse) {
    size_t capacity = MIN_NLINES;
    char **new_arr = malloc(capacity * sizeof(char *));
    char line[MAX_LINE_LEN]; 
    int count = 0;
    size_t p_nelem = 0;
    while (fgets(line, MAX_LINE_LEN, fp)) {
        if (count == capacity) {
            capacity *= 2;
            new_arr = realloc(new_arr, capacity * sizeof(char *));
            assert(new_arr != NULL);
        }
        if (uniq) {
            char *temp = line;
            char **insert = binsert(&temp, new_arr, &p_nelem, sizeof(char *), cmp);
            if (p_nelem > count) {
                *insert = strdup(line);
                count = p_nelem;
            }
        }
        else { // temp is need to be added to arr
            char *copy = strdup(line);
            new_arr[count] = copy;
            count++;
        }    
    }

    if (!uniq) {
        qsort(new_arr, count, sizeof(new_arr[0]), cmp);
    }
    print_and_free(new_arr, count, reverse);      

}

// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

int main(int argc, char *argv[]) {
    cmp_fn_t cmp = cmp_pstr;
    bool uniq = false;
    bool reverse = false;

    int opt = getopt(argc, argv, "lnru");
    while (opt != -1) {
        if (opt == 'l') {
            cmp = cmp_pstr_len;
        } else if (opt == 'n') {
            cmp = cmp_pstr_numeric;
        } else if (opt == 'r') {
            reverse = true;
        } else if (opt == 'u') {
            uniq = true;
        } else {
            return 1;
        }

        opt = getopt(argc, argv, "lnru");
    }

    FILE *fp = stdin;
    if (optind < argc) {
        fp = fopen(argv[optind], "r");
        if (fp == NULL) {
            error(1, 0, "cannot access %s", argv[optind]);
        }
    }
    sort_lines(fp, cmp, uniq, reverse);
    fclose(fp);
    return 0;
}

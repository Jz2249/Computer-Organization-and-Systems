/* This file includes test code for the binsert function implemented as part of
 * assign4. You do not need to modify this file.
 * 
 * You can run this program by specifying the -i flag follwed by integer
 * arguments (sorted numerically), or the -s flag followed by text arguments
 * (sorted alphabetically), and this program will use binsert to construct a
 * sorted array of those elements and print them out.  E.g.
 *
 * ./test_binsert -i 5 61 -2 1
 * ./test_binsert -s hello hi howdy
 */

#include <assert.h>
#include <error.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "samples/prototypes.h"

/* A comparison function for ints that orders numbers in ascending order. */
int cmp_int(const void *p, const void *q) {
    int first = *(const int *)p;
    int second = *(const int *)q;
    return first - second;
}

/* Test building up an array by parsing each string argument as an int
 * in order and inserting it using binsert.  Asserts if each
 * element was inserted/handled correctly, and prints out the
 * built up array at the end.
 */
void test_int(int count, char *args[]) {
    // Use a heap-allocated array because Valgrind can more easily catch errors that way
    int *arr = malloc(sizeof(int) * count);
    assert(arr != NULL);

    size_t nused = 0;

    for (int i = 0; i < count; i++) {
        int val = atoi(args[i]);
        int *added = binsert(&val, arr, &nused, sizeof(arr[0]), cmp_int);
        assert(val == *added);
    }

    for (int i = 0; i < nused; i++) {
        printf(" %d", arr[i]);
    }
    printf("\n");
    free(arr);
}

/* A comparison function for strings that compares strings in alphabetical order
 * in the same way as strcmp.
 */
int cmp_str(const void *p, const void *q) {
    const char *first = *(const char **)p;
    const char *second = *(const char **)q;
    return strcmp(first, second);
}

/* Test building up an array with each string argument
 * inserted in order using binsert.  Asserts if each
 * element was inserted/handled correctly, and prints out the
 * built up array at the end.
 */
void test_string(int count, char *args[]) {
    // Use a heap-allocated array because Valgrind can more easily catch errors that way
    char **arr = malloc(sizeof(char *) * count);
    size_t nused = 0;

    for (int i = 0; i < count; i++) {
        char *val = args[i];
        char **added = binsert(&val, arr, &nused, sizeof(arr[0]), cmp_str);
        assert(strcmp(val, *added) == 0);
    }

    for (int i = 0; i < nused; i++) {
        printf(" %s", arr[i]);
    }
    printf("\n");
    free(arr);
}

int main(int argc, char *argv[]) {
    if (argc == 1 || (strcmp(argv[1], "-s") != 0 && strcmp(argv[1], "-i") != 0)) {
        error(1, 0, "Error: the first argument must be either -i (int) or -s (string).");
    }

    if (strcmp(argv[1], "-s") == 0) {
        test_string(argc - 2, argv + 2);
    } else {
        test_int(argc - 2, argv + 2);
    }

    return 0;
}

/* Overide the printed program name for the error function so that the
 * solution matches the student implementation error messages for invalid flags.
 */
char *program_invocation_name = "test_binsert";

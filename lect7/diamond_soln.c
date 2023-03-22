/**
 * CS107
 *
 * This program prints out the provided command line argument string
 * in a diamond pattern.  For instance, if you provide "doris" as a
 * command-line argument, it will print out the following pattern:
 *
 * d
 * do
 * dor
 * dori
 * doris
 *  oris
 *   ris
 *    is
 *     s
 */

#include <stdio.h>
#include <string.h>
#include <error.h>

void diamond(char *str) {
    size_t length = strlen(str);
    for (size_t i = 1; i < length; i++) {
        char prefix[i + 1];
        strncpy(prefix, str, i);
        prefix[i] = '\0';
        printf("%s\n", prefix);
    }

    printf("%s\n", str);
    // print bottom half of diamond
    for (size_t i = 1; i < length; i++) {
        for (size_t j = 0; j < i; j++) {
            printf(" ");
        }

        printf("%s\n", str + i);
    }
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        error(1, 0, "Usage: ./diamond <text>");
    } else {
        diamond(argv[1]);
    }
    
    return 0;
}

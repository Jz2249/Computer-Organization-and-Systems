/* CS107 Assignment 3
 * Code by Jie Zhu
 * 
 * This program prints the last n lines from the indicated file. 
 * If there is no file, it will read from the standard input.
 * 
 * The number n is indicated by user in command line after '-'.
 * If there is no indicated n, then n will be the default number 10.
 *
 * The last n lines will be stored (may no in order) in an array in print_last function.
 * The print output is using circular queue data structure to print the lines in an order.
 * The first printing line will be the next element after the last line added to the array if number of lines > n.
 */


#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include "samples/prototypes.h"

#define MAX_NUM_LINES 100000
#define DEFAULT_NUM_LINES 10
#define NUMERIC_ARG_BASE 10

/* This function print the last n lines from the indicated file_pointer.
 * If there are less than n lines in the file, this program prints
 * everything line by line in the order. 
 * If there are more than n lines in the file, the output will print
 * the last n lines.
 */
void print_last_n(FILE *file_pointer, int n) 
{
    // your implementation here
    char *lines[n];
    char *line = NULL;
    size_t i = 0; // i is the number of lines have read
    // put the last n lines in the array, but may not in order eg.[line4, line2, line3] -> line2, line3, line4 
    while ((line = read_line(file_pointer)))
    {
        // free the element which will be replaced in the array
        if (i >= n)
        {
            free(lines[i % n]);
        }
        lines[i % n] = line;
        i++;
    }
    // array is not in order
    if (i > n )
    {
        // reassign the index i to the first line which should be printed
        i = i % n;
        for (int j = i; j < i + n; j++)
        {
            line = lines[j % n];
            printf("%s\n", line);
            free(line);
        }
    }
    else
    {
        for (int j = 0; j < i; j++)
        {
            line = lines[j];
            printf("%s\n", line);
            free(line);
        }
    }
}

// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------
/* This function convert the argument str into an integer. 
 * If there is invalid form of integer, the function will return an error.
 */
int convert_arg(const char *str, int max) {
    char *end = NULL;
    long parsed_number = strtol(str, &end, NUMERIC_ARG_BASE);
    if (*end != '\0') {
        error(1, 0, "Invalid number '%s'", str);
    }
    if (parsed_number < 1 || parsed_number > max) {
        error(1, 0, "%s is not within the acceptable range [%d, %d]", str, 1, max);
    }
    return parsed_number;
}
/* This main program implements print_last_n by extracting the number 
 * after '-' and file if have (in command line). 
 * If there is no '-' indicated, the default number of lines will be printed.
 * If there is no file indicated, it will read from standard input.
 * If the reading file is not shown properly, there will be an error.
 */
int main(int argc, char *argv[]) {
    int num_lines = DEFAULT_NUM_LINES;

    if (argc > 1 && argv[1][0] == '-') {
        num_lines = convert_arg(argv[1] + 1, MAX_NUM_LINES);
        argv++;
        argc--;
    }

    FILE *file_pointer = NULL;
    if (argc == 1) {
        file_pointer = stdin;
    } else {
        file_pointer = fopen(argv[1], "r");
        if (file_pointer == NULL) {
            error(1, errno, "cannot access '%s'", argv[1]);
        }
    }

    print_last_n(file_pointer, num_lines);
    fclose(file_pointer);
    return 0;
}

/* CS107 Assignment 3
 * Code by Jie Zhu
 * 
 * This program prints the unique lines from the given file along with number of each unique lines.
 * The print_uniq_lines uses a dynamic arary to store the unique lines with its frequency using 
 * a line_info structure. 
 * Each time when the same line is read, it will accumulate the count number in line_info struct.
 * All the heap memory will be freed at the end.
 */

#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "samples/prototypes.h"


/* initial estimate of number of uniq lines
 * resize-as-you-go, add in increments of 100
 */
#define ESTIMATE 100
typedef struct line_info
{
    char *str;
    int count;
} line_info;

/* This function is used to find out if the specific string cmpstr has been occured in the array.
 * Return value is the ith element in the array which has the same string value as cmpstr.
 * Its return value could be used as index (i - 1) of the array that has the same value as cmpstr.
 * For example, is_same returns 3, then array[3 - 1] is the string who has the same value as cmpstr.
 * If there is no string has the same value, return 0.
 */
int is_same(line_info *array, char *cmpstr, int size)
{
    for (int i = 1; i < size + 1; i++)
    {
        // check if cmpstr is the same string as one of the real element in the array 
        if (strcmp(array[i - 1].str, cmpstr) == 0)
        {
            return i; // the ith element in the array which has the same string value as cmpstr. (its index will be i - 1)
        }
    }
    return 0;
}
/* This function prints the unique lines from the given file_pointer.
 * Along with the lines, the function will also print the frequency of each unique line in the file.
 * Notice: In below, if there is same string found, the value of nth_is_same from is_same function is 
 *          from 1 to the number of total unique line. Thus, the index of its location will be nth_is_same - 1.       
 */
void print_uniq_lines(FILE *file_pointer) 
{
    // your implementation here
    size_t size = ESTIMATE;
    line_info *array = malloc(size * sizeof(line_info));
    assert(array != NULL);
    char *line = NULL;
    // last index of the array
    int i = 0;
    while ((line = read_line(file_pointer)) != NULL)
    {
        // check if we need realloc
        if (i >= size)
        {
            size += ESTIMATE;
            array = realloc(array, size * sizeof(line_info));
            assert(array != NULL);
        }
        // if the nth element is the same string, its index will be nth_is_same - 1, otherwise, it is 0
        int nth_is_same = is_same(array, line, i);
        // if we have the same string
        if (nth_is_same)
        {
            array[nth_is_same - 1].count++;
            free(line);
        }
        else
        {
            array[i].str = line;
            array[i].count = 1;
            i++;
        }
    }
    for (int j = 0; j < i; j++)
    {
        printf("%7d %s\n", array[j].count, array[j].str);
        free(array[j].str);
    }
    free(array);
}


// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

/* This main program implements the function print_uniq_lines to print the unique line
 * of the reading file with its number of same lines. 
 * If there is only one argument in command line, it will read the file through standard input.
 * Otherwise, the file is based on the user given. 
 */
int main(int argc, char *argv[]) {
    FILE *file_pointer = NULL;
    // if only have one argument in commandline, read standard input
    if (argc == 1) {
        file_pointer = stdin;
    } else {
        file_pointer = fopen(argv[1], "r");
        if (file_pointer == NULL) {
            error(1, errno, "cannot access '%s'", argv[1]);
        }
    }

    print_uniq_lines(file_pointer);
    fclose(file_pointer);
    return 0;
}

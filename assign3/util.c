#include "samples/prototypes.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/* initial allocation will be for min size, if not big enough, doubles
 * to 64, then 128, then 256, etc. as needed to accommodate the entire line
 * resize-as-you-go, doubling each time.
 */
#define MINIMUM_SIZE 32

/* This function reads the line from the file_pointer.
 * It will return the read line or NULL if nothing more can be read.
 * This is a more convenient version of fgets.
 */
char *read_line(FILE *file_pointer) 
{
    size_t capacity = MINIMUM_SIZE;
    size_t size = MINIMUM_SIZE;
    char *buf = malloc(capacity);
    assert(buf != NULL);
    int i = 0;
    while (fgets(buf + i, size, file_pointer))
    {
        i = strcspn(buf, "\0");
        char *temp = strstr(buf, "\n");
        // if we don't have enough space to read the whole line
        if (temp == NULL)
        {
            capacity *= 2;
            size = capacity / 2;
            buf = realloc(buf, capacity);
            assert(buf != NULL);
        }
        // if we reach the end of line
        else 
        {
            *temp = '\0';
            return buf;
        }
    }
    // last line doesn't have '\n' or it is an empty line
    if (i != 0) return buf;
    free(buf);
    return NULL;
}




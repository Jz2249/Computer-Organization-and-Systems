#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "samples/prototypes.h"

#define INVERTED_BOX "\e[7m \e[0m"

/* uncomment below to swap in "+" as definition of LIVE_STR if your
 * terminal doesn't play nice with inverted box
 */
#define LIVE_STR INVERTED_BOX
// #define LIVE_STR "+"
#define EMPTY_STR " "
#define DEFAULT_GEN_0 (1L << 32)
#define N_GENERATIONS 32
#define bitmask11 0b11
#define bitmask110 0b110
#define bitmask111 0b111
const int size_i = sizeof(long) * CHAR_BIT - 1;

/*find the specific index element in ruleset and check if it is 1*/
int is_live(unsigned char ruleset, int temp)
{
    unsigned char t=ruleset >> temp;
    return (t & 1) == 1;
}

unsigned long advance(unsigned long cur_gen, unsigned char ruleset) {
    unsigned long copy = 0L;
    for (int i = size_i; i >= 0; i--)
    {
        int neigh;
        if (i == size_i)
        {   
            unsigned long temp = (cur_gen >> (i-1));  // eg 10000-> 00010 
            neigh=temp & bitmask11;
            if (is_live(ruleset,neigh)) copy += (1L << i); // eg 00000-> 10000   

        }
        else if (i == 0)                               // eg 00001
        {
            unsigned long temp = (cur_gen << 1);
            neigh = temp & bitmask110;
            if (is_live(ruleset,neigh)) copy += 1L;
        }
        else                                        // eg 01000-> 00010
        {
            unsigned long temp = (cur_gen >> (i-1)); 
            neigh = temp & bitmask111;    // isolate 3 cells neighborhood at right most 
            if (is_live(ruleset,neigh)) copy += (1L << i); // eg 10000-> 11000 
        }
    }
    return copy;
}

void draw_generation(unsigned long gen) {
    // each cell in gen is drawn as LIVE_STR or EMPTY_STR depending on whether cell is on or off
    for (int i = size_i; i >= 0; i--)
    {
        unsigned long temp = gen >> i;
        if ((temp & 1) == 0)
        {
            printf("%s",EMPTY_STR);
        }
        if ((temp & 1) == 1)
        {
            printf("%s",LIVE_STR);
        }
    }
    printf("\n");
}




// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------


unsigned long convert_arg(const char *str, unsigned long low, unsigned long high, const char *argname) {
    char *end = NULL;
    unsigned long n = strtoul(str, &end, 0);
    if (*end != '\0') {
        error(1, 0, "Invalid number '%s' for %s", str, argname);
    }
    if (n < low || n > high || errno == ERANGE) {
        error(1, 0, "%s %s is not in range [%#lx, %#lx]", argname, str, low, high);
    }
    return n;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        error(1, 0, "Missing argument. Please supply ruleset and optional initial generation.");
    }

    unsigned long gen_0 = DEFAULT_GEN_0;
    unsigned char ruleset = convert_arg(argv[1], 0, UCHAR_MAX, "ruleset");
    if (argc > 2) {
        gen_0 = convert_arg(argv[2], 0, ULONG_MAX, "initial generation");
    }

    if (ruleset == 0) {
        draw_generation(gen_0);
    } else {
        unsigned long cur_gen = gen_0;
        for (int i = 0; i < N_GENERATIONS; i++) {
            draw_generation(cur_gen);
            unsigned long next_gen = advance(cur_gen, ruleset);
            if (next_gen == cur_gen) {
                break;
            }
            cur_gen = next_gen;
        }
    }
    return 0;
}

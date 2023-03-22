#include <errno.h>
#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "samples/prototypes.h"

const int longsize = sizeof(long) * CHAR_BIT;

long signed_max(int bitwidth) {
    long temp = signed_min(bitwidth);
    temp ^= -1;
    return temp;
}

long signed_min(int bitwidth) {
    long temp = -1L << (bitwidth - 1);
    return temp;
}

long sat_add(long operand1, long operand2, int bitwidth) {
    long max = signed_max(bitwidth);
    long min = signed_min(bitwidth);
    long sum = operand1 + operand2;
    // check each operand is positive or negative
    long sign1 = (operand1 >> (bitwidth -1));
    long sign2 = (operand2 >> (bitwidth -1));
    long sum_sign = (sum >> (bitwidth -1));
    sum_sign &= 1;
    sign1 &= 1;
    sign2 &= 1;
    // if both opperands are positive but have negative sum sign 
    if (sign1 == 0 && sign2 == 0)
    {        
        if (sum_sign == 1)
        {
            sum = max;
        }
    }
    // if both opperands are negative but have positive sum sign
    else if (sign1 == 1 && sign2 == 1)
    {
        if (sum_sign == 0)
        {
            sum = min;
        }
    }
    
    return sum;
}




// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------


long convert_arg(const char *str, long low, long high, const char *argname) {
    char *end = NULL;
    long n = strtol(str, &end, 0);
    if (*end != '\0') {
        error(1, 0, "Invalid number '%s'", str);
    }
    if (n < low || n > high || errno == ERANGE) {
        error(1, 0, "Illegal value %s. %s must be in range [%ld, %ld]", str, argname, low, high);
    }
    return n;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        error(1, 0, "Missing argument. Please specify the bitwidth.");
    }

    int bitwidth = convert_arg(argv[1], 4, sizeof(long) * CHAR_BIT, "Bitwidth");
    long min = signed_min(bitwidth);
    long max = signed_max(bitwidth);

    if (argc < 4)  {
        printf("%d-bit signed integer range\n", bitwidth);
        printf("min: %ld   %#018lx\n", min, min);
        printf("max:  %ld   %#018lx\n", max, max);
    } else {
        long sum = convert_arg(argv[2], min, max, "Operand");
        printf("%ld ", sum);
        for (int i = 3; i < argc; i++) {
            long next = convert_arg(argv[i], min, max, "Operand");
            printf("+ %ld ", next);
            sum = sat_add(sum, next, bitwidth);
        }
        printf("= %ld\n", sum);
    }
    return 0;
}


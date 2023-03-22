#include <error.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "samples/prototypes.h"

#define upper_1 0x7f
#define lower_2 0x80
#define upper_2 0x07ff
#define lower_3 0x0800
#define bitmask 0b111111
#define bitmask_110 0b110
#define bitmask_10 0b10
#define bitmask_1110 0b1110


int to_utf8(unsigned short code_point, unsigned char utf8_bytes[]) 
{

    if (code_point <= upper_1)
    {
        utf8_bytes[0] = code_point;
        return 1;
    }
    if (code_point <= upper_2 && code_point >= lower_2)
    {
        unsigned short temp = (bitmask_110 << 5);
        unsigned short five_most = code_point >> 6;
        temp += five_most;
        utf8_bytes[0] = temp;
        temp = (bitmask_10 << 6);
        unsigned short six_remain = code_point & bitmask;
        temp += six_remain; 
        utf8_bytes[1] = temp;
        return 2;
    }
    if (code_point >= lower_3)
    {
        unsigned short temp = (bitmask_1110 << 4);
        unsigned short four_most = code_point >> 12;//at most 16 significant bits
        temp += four_most;
        utf8_bytes[0] = temp;
        temp = (bitmask_10 << 6);
        unsigned short six_next = code_point >> 6;
        six_next &= bitmask;
        temp += six_next;
        utf8_bytes[1]=temp;
        temp = (bitmask_10 << 6);
        unsigned short six_remain = code_point & bitmask;
        temp += six_remain;
        utf8_bytes[2] = temp;
        return 3;
    }
    return 0;
}




// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

void print_utf8(unsigned short code_point, unsigned char utf8_bytes[], int utf8_bytes_length) {
    int i = 0;
    printf("U+%04X   UTF-8 Hex:", code_point);
    for (i = 0; i < utf8_bytes_length; i++) {
       printf(" %02x", utf8_bytes[i]);
    }
    for ( ; i < 4; i++) {
        printf("   ");
    }
    utf8_bytes[utf8_bytes_length] = '\0';
    printf("Character: %s  \n", utf8_bytes);
}

unsigned short convert_arg(const char *str, unsigned short low, unsigned short high) {
    char *end = NULL;
    long n = strtol(str, &end, 0);
    if (*end != '\0') {
        error(1, 0, "Invalid code point '%s'", str);
    }
    if (n < low || n > high) {
        error(1, 0, "Code point %s not in range [%u, %u]", str, low, high);
    }
    return n;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        error(1, 0, "Missing argument. Please supply one or more unicode code points in decimal or hex.");
    }

    for (int i = 1; i < argc; i++) {
        unsigned short code_point = convert_arg(argv[i], 0, USHRT_MAX);
        unsigned char utf8_bytes[4];
        int num_utf8_bytes = to_utf8(code_point, utf8_bytes);
        print_utf8(code_point, utf8_bytes, num_utf8_bytes);
    }
    return 0;
}

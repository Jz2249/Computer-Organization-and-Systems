#include "samples/prototypes.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* This function is an augmented verison of bsearch. Most of its code are from apple_bsearch.
 * Return value is a pointer to the value of key which is the element of which is pointed by base.
 * p_nelem is the address of the number of elements and width is the size of each element.
 * The contents of the return value should be in the order which is referenced by compar function.
 * If key is not found, it will be inserted in the array with position identified by compar function.
 */
void *binsert(const void *key, void *base, size_t *p_nelem, size_t width,
              int (*compar)(const void *, const void *)) {
    // TODO(you!): implement this function (and remove the line below)
    void *start = base;
    void *new = base;
    for (size_t nremain = *p_nelem; nremain != 0; nremain >>= 1) {
        void *p = (char *)base + (nremain >> 1) * width;
        int sign = compar(key, p);
        if (sign == 0) {
            return p;
        }
        if (sign > 0) { /* key > p: move right */
            if (nremain <= 2) {
                // insert position is the next one to p
                new = (char *)p + width;
                break;
            }
            base = (char *)p + width;
            nremain--;
        }
        // key < p at the last search
        if (nremain == 1) {
            // insert position is the current p
            new = p;
            break;
        }
    }
    int prefix_width = (char *)new - (char *)start;
    int suffix_width = (*p_nelem) * width - prefix_width;
    (*p_nelem)++;
    // shift all the elements start from position new to the right once
    memmove((char *)new + width, new, suffix_width);
    memcpy(new, key, width);
    return new;    
}

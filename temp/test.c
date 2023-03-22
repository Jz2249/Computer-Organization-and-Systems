#include "./allocator.h"
#include "./debug_break.h"
#include <stdio.h>
#include <string.h>

#define rm_1bit 0xFFFFFFFFFFFFFFF8

static void *segment_start;
static void *segment_end;
static size_t segment_size;
static size_t nused;
static size_t num_header;


size_t roundup(size_t sz, size_t mult) {
    return (sz + mult - 1) & ~(mult - 1);
}

bool myinit(void *heap_start, size_t heap_size) {
    /* TODO(you!): remove the line below and implement this!
     * This must be called by a client before making any allocation
     * requests.  The function returns true if initialization was
     * successful, or false otherwise. The myinit function can be
     * called to reset the heap to an empty state. When running
     * against a set of of test scripts, our test harness calls
     * myinit before starting each new script.
     */
    if (heap_size > MAX_REQUEST_SIZE) {
        segment_start = heap_start;
        segment_size = heap_size;
        nused = ALIGNMENT;
        num_header = 1;
        segment_end = heap_start;
        *(size_t *)segment_start = heap_size - ALIGNMENT;
        return true;
    }
    return false;
}
/* Check the header to see if it is freed
 */
bool is_free(void *ptr) {
    int status = (*(size_t *)ptr) & 0x1;
    if (status == 0) return true;
    return false;
}

void *mymalloc(size_t requested_size) {
    // TODO(you!): remove the line below and implement this!
    size_t needed = roundup(requested_size, ALIGNMENT);
    // if (needed + nused > segment_size) {
    //     return NULL;
    // }
    void *header = segment_start;
    int n = 0;
    while (header <= segment_end) {
        n++; // track the number of header;
        size_t blk_size = (*(size_t *)header) & rm_1bit; // change the lsb to 0 if it is 1
        if (!is_free(header) || blk_size < needed) {
            header = (char *)header + ALIGNMENT + blk_size;
            continue;
        }
        // freed block
        // if (blk_size < needed) {
        //     header = (char *)header + ALIGNMENT + blk_size;
        //     continue;
        // }
        // find the correct position, check if it is the last header
        if (n >= num_header) {
            *(size_t *)header = needed | 0x1;
            segment_end = (char *)header + ALIGNMENT + needed;
            void *end_blk = (char *)segment_start + segment_size - ALIGNMENT;
            if ((char *)segment_end >= (char *)end_blk) { // no more space
                *(size_t *)header = ((char *)end_blk - (char *)header ) | 0x1;
                // nused += (*(size_t *)header & rm_1bit);
            }
            else { // has more space for the next header
                // nused += (*(size_t *)header & rm_1bit) + ALIGNMENT;
                void *temp = segment_end;
                *(size_t *)temp = ((char *)end_blk - (char *)header);
                num_header++;
            }
        }
        else { // position is one of the previous used but has freed block
            *(size_t *)header = blk_size | 0x1;
            // nused += blk_size;
            // nused += (*(size_t *)header & rm_1bit);
        }
        void *curr = (char *)header + ALIGNMENT;
        return curr;
    }
    return NULL;
}



void myfree(void *ptr) {
    // TODO(you!): implement this!
    void *header = (char *)ptr - ALIGNMENT;
    // if (header < segment_start || (char *)header > (char *)segment_start + nused) {
    //     // printf("invalid");
    //     return;
    // }
    size_t blk_size = (*(size_t *)header) & rm_1bit;
    // nused -= blk_size;
    *(size_t *)header = blk_size;
    // void *next_header = (char *)ptr + blk_size;
    // int next_status = *(size_t *)next_header & 0x1; // check if next header is freed or used
    // if (next_status == 0 && (char *)next_header <= (char *)segment_start +nused) { // freed
    //     *(size_t *)header = blk_size + *(size_t *)next_header +ALIGNMENT;
    // }
    // else { // next block is in used
    //     *(size_t *)header = blk_size;
    // }
}

/* Find the new position that is free and can store size of new_size,
 * The function will return the pointer of the new position. 
 */
// void *find_position(void *ptr, size_t new_size) {
//     void *header = (char *)ptr - ALIGNMENT;
//     size_t blk_size = *(size_t *)header & rm_1bit;
//     int n = 0;
//     while (header <= segment_end) {
//         if (!is_free(header)) {
//             header = (char *)header + ALIGNMENT + blk_size;

//         }

//     }
// }
void *myrealloc(void *old_ptr, size_t new_size) {
    // TODO(you!): remove the line below and implement this!
    size_t needed = roundup(new_size, ALIGNMENT);
    void *new_ptr = mymalloc(needed);
    if (new_ptr != NULL) {
        void *old_header = (char *)old_ptr - ALIGNMENT;
        size_t old_size = (*(size_t *)old_header) & rm_1bit;
        if (old_size <= needed) {
            memmove(new_ptr, old_ptr, old_size);
        }
        else {
            memmove(new_ptr, old_ptr, needed);
        }
        void *new_header = (char *)new_ptr - ALIGNMENT;
        *(size_t *)new_header |= 0x1;
        // nused += needed + ALIGNMENT;
        myfree(old_ptr);
    }
    return new_ptr;
}

bool validate_heap() {
    /* TODO(you!): remove the line below and implement this to
     * check your internal structures!
     * Return true if all is ok, or false otherwise.
     * This function is called periodically by the test
     * harness to check the state of the heap allocator.
     * You can also use the breakpoint() function to stop
     * in the debugger - e.g. if (something_is_wrong) breakpoint();
     */
    return true;
}

/* Function: dump_heap
 * -------------------
 * This function prints out the the block contents of the heap.  It is not
 * called anywhere, but is a useful helper function to call from gdb when
 * tracing through programs.  It prints out the total range of the heap, and
 * information about each block within it.
 */
void dump_heap() {
    // TODO(you!): Write this function to help debug your heap.
}

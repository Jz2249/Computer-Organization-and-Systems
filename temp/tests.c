#include "./allocator.h"
#include "./debug_break.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_SIZE 24
#define RM_1BIT 0xFFFFFFFFFFFFFFF8


static void *segment_start;
static void *segment_end;
static void *head_max;
static size_t segment_size;
static void *segment_free; // first free header
// static size_t nused;
static size_t num_header;
#define BYTES_PER_LINE 32


size_t roundup(size_t sz, size_t mult) {
    return (sz + mult - 1) & ~(mult - 1);
}

bool is_free(void *ptr) {
    int status = (*(size_t *)ptr) & 0x1;
    if (status == 0) return true;
    return false;
}

char *get_prev(char *ptr) {
    char *temp = (char *)*(size_t *)(ptr + ALIGNMENT);
    if (temp < (char *)segment_start ) return NULL;
    return temp;
}
char *get_next(char *ptr) {
    char *temp = (char *)*(size_t *)(ptr + 2 *ALIGNMENT);
    if (temp > (char *)segment_end + segment_size) return NULL;
    return temp;
}
void set_prev(char *ptr, char *prev) {
    *(size_t *)(ptr + ALIGNMENT) = (size_t)prev;
}
void set_next(char *ptr, char *next) {
    *(size_t *)(ptr + 2 * ALIGNMENT) = (size_t)next;
}
size_t get_sz(char *n) {
    size_t size = (*(size_t *)n) & RM_1BIT;
    return size;
}
void *get_right(void *ptr) {
    size_t blksz = get_sz(ptr);
    void *right = (char *)ptr + ALIGNMENT + blksz;
    return right;
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
    if (heap_size >= MAX_REQUEST_SIZE) {
        segment_start = heap_start;
        *(size_t *)segment_start = heap_size - ALIGNMENT;
        segment_size = heap_size;
        segment_end = segment_start;
        segment_free = segment_start;
        head_max = (char *)segment_start + segment_size - MIN_SIZE;
        set_prev(segment_free, NULL);
        set_next(segment_free, NULL);
        num_header = 1;
        return true;
    }
    return false;
}


void rm_free(char *curr, size_t sz, char *curr_next, char *curr_prev) {
    if (curr_prev != NULL && curr_next != NULL) {
        set_prev(curr_next, curr_prev);
        set_next(curr_prev, curr_next);
    }
    else if (curr_prev != NULL && curr_next == NULL) {
        set_next(curr_prev, curr_next);
    }
    else if (curr_prev == NULL && curr_next != NULL) {
        set_prev(curr_next, curr_prev);
    }
    else {
        segment_free = NULL;
    }
    if (curr == (char *)segment_free) {
        segment_free = curr_next;
    }
}

void add_free(char *curr, size_t sz) {
    char *temp = (char *)head_max - MIN_SIZE; // the last head that can add a new head to the right
    size_t blksz = get_sz(curr);
    char *new = curr + ALIGNMENT + blksz;
    size_t blk_left = (char *)segment_start + segment_size - new - ALIGNMENT;
    char *prev = get_prev(curr);
    char *next = get_next(curr);
    if (curr <= temp) { // can add free new header
        *(size_t *)new = blk_left;
        rm_free(curr, sz, new, prev);
        set_next(new, NULL);
    }
    else {
        *(size_t *)curr += blk_left;
        rm_free(curr, blk_left, next, prev);
    }
    *(size_t *)curr = sz | 0x1;
}

void *mymalloc(size_t requested_size) {
    // TODO(you!): remove the line below and implement this!
    size_t needed = roundup(requested_size, ALIGNMENT * 2);
    char *curr = segment_free;
    int num = 0; // trace the num of head
    while (curr != NULL) {
        num++;
        size_t blksz = get_sz(curr);
        if (blksz < needed) {
            curr = get_next(curr); // go to the next freed head
            continue;
        }
        if (num >= num_header) {
            *(size_t *)curr = needed;
            add_free(curr, needed); // add free to the right, if it cannot add more header, update the current header

        }
        else { // position is within the previously used block
            char *prev = get_prev(curr);
            char *next = get_next(curr);
            rm_free(curr, blksz, next, prev); 
            *(size_t *)curr = blksz | 0x1;
            num_header--;
        }
        void *cur = curr + ALIGNMENT;
        return cur;
    }
    return NULL;
}



void coalesc(void *ptr) {
    size_t blksz = get_sz(ptr);
    char *right = (char *)ptr + ALIGNMENT + blksz;
    if(is_free(right)) {
        size_t right_blksz = get_sz(right);
        *(size_t *)ptr += (right_blksz + ALIGNMENT); 
        char *right_next = get_next(right);
        rm_free(right, right_blksz, right_next, ptr);
        num_header--;
    }
}
void insert_free(void *ptr) {
    char *temp = segment_free;
    void *prev = NULL;
    int i = 1;
    while (i <= num_header) {
        prev = get_prev(temp);
        if (temp > (char *)ptr) { // temp is the next
            if (prev != NULL) {
                set_next(prev, ptr);
            }
            set_prev(temp, ptr);
            break;
        }
        temp = get_next(temp);
        i++;
    }
    if (i > num_header) {
        if (prev != NULL) set_next(prev, ptr);
    }
    set_next(ptr, temp);
    set_prev(ptr, prev); 
    if ((char *)ptr < (char *)segment_free) segment_free = ptr;
    *(size_t *)ptr &= RM_1BIT;
} 


void myfree(void *ptr) {
    // TODO(you!): implement this!
    char *header = (char *)ptr - ALIGNMENT;
    if (is_free(header)) return;
    *(size_t *)header &= RM_1BIT;
    // if (header < (char *)segment_free) segment_free = header;
    insert_free(header);
    num_header++; // ******** && segment_free
    coalesc(header);
  
}
bool can_coalesc(void *ptr) {
    char *right = get_right(ptr);
    if(is_free(right)) {
        return true;
    }
    return false;
}
/* find the large enough block and return the header address
 */
void *find_position(size_t needed) {
    void *temp = segment_free;
    size_t blksz = get_sz(temp);
    while (blksz < needed) {
        while (can_coalesc(temp)) {
            coalesc(temp);
            blksz = get_sz(temp);
            if (blksz >= needed) return temp; 
        }
        // still not enough space
        temp = get_next(temp);
        blksz = get_sz(temp);
        if (temp == NULL) return NULL;
    }
    return temp;
}

void split(void *ptr, size_t sz, size_t total) {
    size_t extra = total - sz - ALIGNMENT;
    char *new_header= (char *)ptr + ALIGNMENT + sz;
    *(size_t *)new_header = extra;
    insert_free(new_header);
    *(size_t *)ptr = sz | 0x1;
    num_header++;


    // // original code
    // char *new = (char *)ptr + ALIGNMENT + sz;
    // char *right = get_right(ptr);
    // char *prev = get_prev(right);
    // char *next = get_next(right);
    // size_t right_blksz = get_sz(right);
    // rm_free(right, right_blksz, new, prev);

    // size_t blk_left = total - sz - ALIGNMENT;
    // *(size_t *)new = blk_left;
    // *(size_t *)ptr = sz;
    // char *next = get_next(ptr);
    // set_next(ptr, new);
    // set_prev(new, ptr);
    // set_next(new, next);
    // if (next != NULL) {
    //     set_prev(next, new);
    // }
    // num_header++;
}

bool can_grow(void *ptr, size_t needed) {
    char *right = get_right(ptr);
    size_t blksz = get_sz(ptr);
    while (is_free(right)) {
        size_t right_blksz = get_sz(right);
        blksz += right_blksz + ALIGNMENT;
        if (blksz >= needed) {
            return true;
        }
        right = get_right(right);
    }
    return false;
}

void *grow(void *ptr, size_t needed) {
    while(can_grow(ptr, needed)) { // coalesc all the block needed besides the original one
        void *right = get_right(ptr);
        size_t right_blksz = get_sz(right);
        size_t blksz = get_sz(ptr);
        size_t total = blksz + right_blksz + ALIGNMENT;
        if (total - needed >= MIN_SIZE) {
            split(ptr, needed, total);
        }
        else *(size_t *)ptr = total;
        // char *prev = get_prev(right);
        // char *next = get_next(right);
        // rm_free(right, right_blksz, next, prev);
        // num_header--;
        if (*(size_t *)ptr >= needed) {
            *(size_t *)ptr |= 0x1;
            return ptr;
        }
    }
    return NULL;
}


void *myrealloc(void *old_ptr, size_t new_size) {
    // TODO(you!): remove the line below and implement this!

    void *old_header = (char *)old_ptr - ALIGNMENT;
    size_t blksz = get_sz(old_header);
    size_t needed = roundup(new_size, ALIGNMENT * 2);
    void *new_ptr = NULL;
    size_t extra;
    // myfree(old_ptr);
    if (needed <= blksz) { // shrink
        if (blksz - needed < MIN_SIZE) {
            return old_ptr;
        }
        // *(size_t *)old_header = needed | 0x1;
        new_ptr = (char *)old_header + ALIGNMENT;
        extra = blksz - needed;
    //     // // test code
        split(old_header, needed, blksz);
        // char *zzz = (char *)new_ptr + needed;
        // *(size_t *)zzz = extra;
        // insert_free(zzz);
        // num_header++;
        return new_ptr;
    }
    // else {
    //     void *temp = grow(old_header, needed);
    //     if (temp != NULL) { // successfully grow at the origin pointer
    //         return (char *)temp + ALIGNMENT;
    //     }
        else {
            new_ptr = mymalloc(needed);
            if (needed > blksz) memcpy(new_ptr, old_ptr, blksz);
            else memcpy(new_ptr, old_ptr, needed);
            blksz = get_sz((char *)new_ptr - ALIGNMENT);
            myfree(old_ptr);
        }
    // }
    // check if need split
    extra = blksz - needed;
    if (blksz - needed >= MIN_SIZE) {
        split((char *)new_ptr - ALIGNMENT, needed, blksz);
        // char *new_free = (char *)new_ptr + needed;
        // *(size_t *)new_free = extra;
        // insert_free(new_free);
      
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
     printf("Heap segment starts at address %p, ends at %p. ",
        segment_start, (char *)segment_start + segment_size);
    for (int i = 0; i < num_header; i++) {
        unsigned char *cur = (unsigned char *)segment_start + i;
        if (i % BYTES_PER_LINE == 0) {
            printf("\n%p: ", cur);
        }
        printf("%02x ", *cur);
    }
}

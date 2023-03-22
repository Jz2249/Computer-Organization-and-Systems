#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* A typedef is a way to give a nickname to a type that is easier to use.  For example,
 * this typedef means you can now use filter_fn_t as the type for a variable, parameter,
 * or return type of this type, in place of the longhand form.  This can come in
 * particularly handy when storing functions in variables.  E.g. instead of
 *
 * int (*myfunc)(const struct_dirent *) = my_fn_name;
 *
 * you can do:
 *
 * filter_fn_t myfunc = my_fn_name;
 *
 * See your C reference or web search for more on typedef.  Consider making other
 * typedefs where appropriate to make your code cleaner!
 */
typedef int (*filter_fn_t)(const struct dirent *);   //typedef char* string

/* Enums are used here as a way to declare 4 integer constants we can use
 * in our program.  This gives us similar benefits to #defining each of these
 * 4 integer constants, but enums allow us to group the constants
 * together and auto-assign them values, since we don't really care what value
 * they are, we just care that they are assigned distinct values and that
 * we can use them in our code.  See your C reference or web search for more on enums!
 */
enum { SORT_BY_NAME, SORT_BY_TYPE };
enum { EXCLUDE_DOT, INCLUDE_DOT };

/* This fully implemented function returns whether the dirent pointed to by
 * the given pointer represents a directory.  (Note: on the myth filesystem,
 * the only file type information that is accurate is directory/not-directory
 * used here. Other type info in struct dirent is not reliable).  Note that
 * this implementation categorizes symbolic links, such as the samples/ directory
 * in the assignment folder, as not a directory, which is fine, and this matches
 * the behavior of the sample solution.
 */
bool is_dir(const struct dirent *dirptr) {
    return dirptr->d_type == DT_DIR;
}

int include_dot(const struct dirent *entry) {
    return entry->d_name[0] == '.';
}
int exclude_dot(const struct dirent *entry) {
    return !include_dot(entry);
}
int cmp_name(const struct dirent **name1, const struct dirent **name2) {
    return strcmp(name1->d_name, name2->d_name);
}
int cmp_type(const struct dirent **name1, const struct dirent **name2) {
    if (is_dir(name1) && !is_dir(name2)) return -1;
    else if (is_dir(name2) && !is_dir(name1)) return 1;
    else return cmp_name(name1, name2);
}

void ls(const char *dirpath, int filter, int order) {
    // TODO(you!): implement this function
    struct dirent **namelist;
    int n;
    if (filter == EXCLUDE_DOT && order == SORT_BY_NAME) {
        n = scandir(dirpath, &namelist, exclude_dot, cmp_name);
    }
    if (filter == EXCLUDE_DOT && order == SORT_BY_TYPE) {
        n = scandir(dirpath, &namelist, exclude_dot, cmp_type);
    }
    if (filter == INCLUDE_DOT && order == SORT_BY_NAME) {
        n = scandir(dirpath, &namelist, include_dot, cmp_name);
    }
    if (filter == INCLUDE_DOT && order == SORT_BY_TYPE) {
        n = scandir(dirpath, &namelist, include_dot, cmp_type);
    }
    if (n == -1)
    {
        error(0, 0, "cannot access %s", argv[1]);
    }
    while (n--)
    {
        printf("%s\n", namelist[n]->d_name);
        free(namelist[n]);
    }
    free(namelist);
}

// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------

int main(int argc, char *argv[]) {
    int order = SORT_BY_NAME;
    int filter = EXCLUDE_DOT;

    int opt = getopt(argc, argv, "az");
    while (opt != -1) {
        if (opt == 'a') {
            filter = INCLUDE_DOT;
        } else if (opt == 'z') {
            order = SORT_BY_TYPE;
        } else {
            return 1;
        }

        opt = getopt(argc, argv, "az");
    }
    
    if (optind < argc - 1) {
        for (int i = optind; i < argc; i++) {
            printf("%s:\n", argv[i]);
            ls(argv[i], filter, order);
            printf("\n");
        }
    } else {
        ls(optind == argc - 1 ? argv[optind] : ".", filter, order);
    }
    
    return 0;
}

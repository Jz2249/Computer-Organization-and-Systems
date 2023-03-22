#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>

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
typedef int (*filter_fn_t)(const struct dirent *); 
typedef int (*order_fn_t)(const struct dirent **, const struct dirent **); 

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

int exclude_dot(const struct dirent *entry) {
    return entry->d_name[0] != '.';
}
int cmp_name(const struct dirent **name1, const struct dirent **name2) {
    return strcmp((*name1)->d_name, (*name2)->d_name);
}
int cmp_type(const struct dirent **name1, const struct dirent **name2) {
    if (is_dir(*name1) && !is_dir(*name2)) return -1; // directory name1 should be before name2
    else if (is_dir(*name2) && !is_dir(*name1)) return 1; // directory name2 should be before name1
    else return cmp_name(name1, name2);
}

/* This function will output the entries from the directory dirpath
 * dirpath is the path this function will search through, if there is no indicated path,
 * this function will search through the current directory.
 * 
 * filter is used to indicate if this function will include or exclude the output beginning with 'dot' 
 * order is used to indicate how the function will sort the output.
 */
void ls(const char *dirpath, int filter, int order) {
    // TODO(you!): implement this function
    // set variable 
    filter_fn_t filt;
    order_fn_t ord;

    if (filter != EXCLUDE_DOT) {
        filt = NULL;
    }
    else {
        filt = exclude_dot;
    }
    if (order == SORT_BY_NAME ) {
        ord = cmp_name;
    }
    else {
        ord = cmp_type;
    }
    struct dirent **namelist = NULL;
    int n = 0;
    n = scandir(dirpath, &namelist, filt, ord);
    if (n == -1) {
        error(0, 0, "cannot access %s", dirpath);
    }
    for (int i = 0; i < n; i++) {
        if (is_dir(namelist[i])) {
            printf("%s/\n", namelist[i]->d_name);
        }
        else {
            printf("%s\n", namelist[i]->d_name);
        }
        free(namelist[i]);
    }
    free(namelist);
}



// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------
/* This program parses the command line arguments as the first parameter in function ls using function getopt.
 * The getopt function will provide a variable optind which is an index to look for the search directory.
 * If there is no argument extracted by optind, this program will apply a default directory ".".
 * With optind provided, getopt function also helps track if the command line  argument has  "-a" or "-z" as filter and order
 */
int main(int argc, char *argv[]) {
    int order = SORT_BY_NAME;
    int filter = EXCLUDE_DOT;
    // search for "-a" and "-z"
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
    // the index of the next element of the argv array to be processed
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

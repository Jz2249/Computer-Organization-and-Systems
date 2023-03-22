/* CS107 Assignment 2
 * Code by Jie Zhu (jz788)
 * 
 * This main function prints the full path to the first matching executable it finds.
 * scan_token and get_env_value are used to implement the program.
 *
 * If there are no additional command-line arguments, the program prints out the directories 
 * in the search path, one per line.
 * For each command name, it will print nothing if no matching executable was found.
 * The matched executables are listed one per line in the order that the command names 
 * were specified on the command-line. 
 */

#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "samples/prototypes.h"


int main(int argc, char *argv[], const char *envp[]) {
    const char *searchpath = get_env_value(envp, "MYPATH");
    // check if the user contains a value for MYPATH
    if (searchpath == NULL) {
        searchpath = get_env_value(envp, "PATH");
    }
    // store the path output
    char dir[PATH_MAX];
    const char *remaining = searchpath;
    // if no command line arguments, prints the directiories in search path
    if (argc == 1) {
        printf("Directories in search path:\n");
        while (scan_token(&remaining, ":", dir, sizeof(dir))) {
            printf("%s\n", dir);
        }
    }
    // if having command line arguments
    if (argc > 1){
        // iterate each excuteable file in command line
        for (int i = 1; i < argc; i++) {
            remaining = searchpath;
            // enough space for / and the length of excutable file name 
            char temp[1 + strlen(argv[i]) + 1];
            strcpy(temp, "/");
            // new string with '/' prepended eg. /emacs
            strcat(temp, argv[i]);
            while (scan_token(&remaining, ":", dir, sizeof(dir))) {
                strcat(dir, temp);
                int can_access = access(dir,  R_OK | X_OK);
                // if can access to the file
                if (can_access != -1) {
                    printf("%s\n", dir);
                    break;
                }
            }
        }
    }
    
    return 0;
}

#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include "samples/prototypes.h"


int main(int argc, char *argv[], const char *envp[]) {
    const char *searchpath = get_env_value(envp, "MYPATH");
    if (searchpath == NULL) {
        searchpath = get_env_value(envp, "PATH");
    }
    
    if (argc == 1) {
        char dir[PATH_MAX];
        const char *remaining = searchpath;

        printf("Directories in search path:\n");
        while (scan_token(&remaining, ":", dir, sizeof(dir))) {
            printf("%s\n", dir);
        }
    }
    
    return 0;
}

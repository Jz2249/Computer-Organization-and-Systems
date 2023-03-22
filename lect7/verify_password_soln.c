/*
 * This program verifies a user's entered password to ensure it adheres
 * to certain criteria.  Specifically, it verifies that the password
 * uses only a certain subset of characters, and that it does not contain
 * any substrings from a provided list of disallowed strings.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <error.h>
#include <stdbool.h>

/* This function returns whether or not the provided password is valid.
 * A valid password contains only characters in `validChars`, and does
 * not contain any of the substrings in the `badSubstrings` array.
 */
bool verifyPassword(char *password, char *validChars,
                    char *badSubstrings[], size_t count) {
    size_t spanLength = strspn(password, validChars);
    if (spanLength != strlen(password)) {
        return false;
    }
    
    for (size_t i = 0; i < count; i++) {
        if (strstr(password, badSubstrings[i]) != NULL) {
            return false;
        }
    }
    
    return true;
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        error(1, 0, "Please provide the password you would like to verify\n");
    } else {
        char *badSubstrings[] = {
            "password",
            "secret"
        };
        
        bool valid = verifyPassword(argv[1],
                                    "abcdefghijklmnopqrstuvwxyz0123456789",
                                    badSubstrings, 2);
        if (valid) {
            printf("That password is valid!\n");
        } else {
            printf("That is an invalid password.\n");
        }
    }

    return 0;
}

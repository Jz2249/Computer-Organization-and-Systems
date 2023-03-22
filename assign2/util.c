#include <string.h>
#include "samples/prototypes.h"

/* Function: get_env_value
 * --------------------------------------
 * This function returns a pointer to the portion of the path based on given key
 * in the element of envp array. 
 * It will search the array envp for the element corresponding to key 
 * and return its value or return NULL if non-exist. 
 * 
 */
const char *get_env_value(const char *envp[], const char *key) {
    // create a temp string contain key value with '='
    size_t key_len = strlen(key);
    char temp[key_len + 1];
    temp[key_len + 1] = '\0';
    temp[key_len] = '=';
    strncpy(temp, key, key_len);

    for (int i = 0; envp[i] != NULL; i++)
    {
        if (strncmp(envp[i], temp, key_len + 1) == 0)
        {
            const char *s = envp[i] + key_len + 1;
            return s;
        }
    }
    return NULL;
}

/* Function scan_token
 * --------------------------------------
 * This function is an improved version of strtok. 
 * It breaks a string into a series of tokens by delimiters.
 * Each call on this function will store the token to the buffer buf. 
 *If there is no token can be stored, the function will return false. 
 */
bool scan_token(const char **p_input, const char *delimiters, char buf[], size_t buflen) {
    *p_input += strspn(*p_input, delimiters);
    // length of the token
    size_t len_token = strcspn(*p_input, delimiters);
    // no token can be extracted
    if (len_token <= 0) 
    {    
        return false;
    }
    // length of token is larger than our capacity, we will truncate it
    if (len_token > buflen -1)
    {
        buf = strncpy(buf, *p_input, buflen - 1);
        buf[buflen - 1] = '\0';
        *p_input += buflen - 1;
    }
    if (len_token > 0 && len_token <= buflen - 1)
    {
        buf = strncpy(buf, *p_input, len_token);
        buf[len_token] = '\0';
        *p_input += len_token;
    }
    return true;
}

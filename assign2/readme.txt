File: readme.txt
Assignment: assign2
Author: <Jie Zhu>
Pronouns: <Jie>
----------------------

scan_token DOCUMENTATION
INSTRUCTIONS: Fill in the sections marked with a TODO below.
Your documentation should be original (i.e., please do not copy and paste from the assignment spec).
NAME
    scan_token - # Break a string into a series of tokens by delimiters.
    bool scan_token(const char **p_input, const char *delimiters,
                    char buf[], size_t buflen);
ARGUMENTS
    const char **p_input - #TODO: This is a pointer to the string *p_input which we want to tokenize.
    const char *delimiters - #TODO: This is a C string that contains delimiters.
    char buf[] - #TODO: This is a buffer that will store the tokenized string and it can store at most bufen - 1 number of characters.
    size_t buflen - #TODO: This is the size of the buffer.
RETURN VALUE
    #TODO: This function will return true if there is a token stored in the buffer buf. 
           If there is no more token stored in the buffer, the function will return false.         
ASSUMPTIONS
    #TODO: The function assumes that the buflen argument is always greater than 1.
           The function assumes that p_input is always a vaild pointer to a pointer.
           The function assumes that *p_input is always a well-formed C-string and it is possibly an empty string.
           The function assumes taht delimiters is always a well-formed C-string containing one or more delimiter characters, 
           but it will never be the empty. 
DESCRIPTION
    #TODO: The scan_token function breaks a string into a series of tokens by delimiters. On the first call to scan_token(),
           the address of the string which is intended to parse should be specified in p_input. In each subsequent call, 
           p_input will point to the substring of the previous string. The delimiters argument specifies a set of bytes that
           delimit the tokens in the parsed string. The buflen argument is the size of the buffer buf including NULL terminator. 
           The buf argument will store the token on each call until there is nothing can be stored and the function will return false. 
           Otherwise, the function will return true. Each call on scan_token function, the pointer p_input will first point to the address
           of the first non-delimiter character. Then, the pointer p_input will point to find the first character contained in delimiters.
           If the length between the two steps above less than 1, the function will return false since there is no token can be scanned.
           Otherwise, the tokenized string will be stored in the buffer buf. If the length of token is larger than buflen - 1, this 
           the pointer p_input will stop scan at the buflen - 1 distance to its initial postion and store the token into the buffer buf. 
           If the length of token is less than the required size, it will store the entire token into buffer. In the last call of scan_token
           function, the address of p_input will point to the address of NULL terminator in the string. 

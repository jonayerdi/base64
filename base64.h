
#ifndef _BASE64_H
#define _BASE64_H

#include <stdio.h>

#define BASE64_ERROR_READING -1
#define BASE64_ERROR_WRITING -2
#define BASE64_ERROR_DECODE -3

/* !! Must be a multiple of 3 !! */
#define BASE64_BUFFER_SIZE 3072

int base64_encode(FILE *input, FILE *output);
int base64_decode(FILE *input, FILE *output);

#endif /* _BASE64_H */

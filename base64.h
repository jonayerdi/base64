
#ifndef _BASE64_H
#define _BASE64_H

#include <stddef.h>
#include <stdio.h>

#define BASE64_ENCODED_SIZE(DECODED_SIZE) ((((DECODED_SIZE) + 2) / 3) * 4)
#define BASE64_DECODED_SIZE(ENCODED_SIZE) (((ENCODED_SIZE) / 4) * 3)

#define BASE64_ERROR_READING -1
#define BASE64_ERROR_WRITING -2
#define BASE64_ERROR_DECODE_BYTE -3
#define BASE64_ERROR_DECODE_SIZE -4

/* !! Must be a multiple of 3 !! */
#define BASE64_ENCODE_BUFFER_SIZE 3072
/* !! Must be a multiple of 4 !! */
#define BASE64_DECODE_BUFFER_SIZE 512

int base64_encode(const char *input, size_t size, char *output);
int base64_decode(const char *input, size_t size, char *output, size_t *output_size);

int base64_encode_file(FILE *input, FILE *output);
int base64_encode_file2(FILE *input, FILE *output);
int base64_decode_file(FILE *input, FILE *output);

#endif /* _BASE64_H */

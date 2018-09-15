/***********************************************************************************
zlib License

Copyright (c) 2017 Jon Ayerdi

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
***********************************************************************************/
#ifndef _BASE64_FILE_H
#define _BASE64_FILE_H

#include "base64.h"
#include <stdio.h> /* FILE */

#define BASE64_ERROR_READING -3
#define BASE64_ERROR_WRITING -4

/* !! Must be a multiple of 3 !! */
#define BASE64_ENCODE_BUFFER_SIZE 3072
/* !! Must be a multiple of 4 !! */
#define BASE64_DECODE_BUFFER_SIZE 4096

int base64_encode_file(FILE *input, FILE *output);
int base64_decode_file(FILE *input, FILE *output, size_t *error_offset);

#endif /* _BASE64_FILE_H */

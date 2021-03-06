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

#ifndef _BASE64_H
#define _BASE64_H

#include <stddef.h> /* size_t, NULL */

#define BASE64_ENCODED_SIZE(DECODED_SIZE) ((((DECODED_SIZE) + 2) / 3) * 4)
#define BASE64_DECODED_SIZE(ENCODED_SIZE) (((ENCODED_SIZE) / 4) * 3)

#define BASE64_OK 0
#define BASE64_ERROR_DECODE_INVALID_BYTE -1
#define BASE64_ERROR_DECODE_INVALID_SIZE -2

void base64_encode_aligned(const char * restrict input, size_t size, char * restrict output);
void base64_encode(const char * restrict input, size_t size, char * restrict output);
int base64_decode(const char * restrict input, size_t size, char * restrict output, size_t *output_size, size_t *error_offset);

#endif /* _BASE64_H */

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

#include "base64.h"

char const *const base64_encode_lookup 
	= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define NA (-1) /* Invalid base64 bytes */
#define PADDING (0) /* Padding character '=' */
int const base64_decode_lookup[] 
	= { NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, 62, NA, NA, NA, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, NA, NA, NA, PADDING, NA, NA
	, NA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, NA, NA, NA, NA, NA
	, NA, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, NA, NA, NA, NA, NA
    , NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA };

#define BASE64_PADDING_CHAR ('=')

#define BASE64_ENCODE_3(BUFF_IN, BUFF_OUT) \
{ \
	(BUFF_OUT)[0] = base64_encode_lookup[(unsigned char)(((BUFF_IN)[0] & 0xFC)>>2)]; \
	(BUFF_OUT)[1] = base64_encode_lookup[(unsigned char)((((BUFF_IN)[0] & 0x03)<<4) | (((BUFF_IN)[1] & 0xF0)>>4))]; \
	(BUFF_OUT)[2] = base64_encode_lookup[(unsigned char)((((BUFF_IN)[1] & 0x0F)<<2) | (((BUFF_IN)[2] & 0xC0)>>6))]; \
	(BUFF_OUT)[3] = base64_encode_lookup[(unsigned char)(((BUFF_IN)[2] & 0x3F)>>0)]; \
}

#define BASE64_ENCODE_2(BUFF_IN, BUFF_OUT) \
{ \
	(BUFF_OUT)[0] = base64_encode_lookup[(unsigned char)(((BUFF_IN)[0] & 0xFC)>>2)]; \
	(BUFF_OUT)[1] = base64_encode_lookup[(unsigned char)((((BUFF_IN)[0] & 0x03)<<4) | (((BUFF_IN)[1] & 0xF0)>>4))]; \
	(BUFF_OUT)[2] = base64_encode_lookup[(unsigned char)(((BUFF_IN)[1] & 0x0F)<<2)]; \
	(BUFF_OUT)[3] = BASE64_PADDING_CHAR; \
}

#define BASE64_ENCODE_1(BUFF_IN, BUFF_OUT) \
{ \
	(BUFF_OUT)[0] = base64_encode_lookup[(unsigned char)(((BUFF_IN)[0] & 0xFC)>>2)]; \
	(BUFF_OUT)[1] = base64_encode_lookup[(unsigned char)(((BUFF_IN)[0] & 0x03)<<4)]; \
	(BUFF_OUT)[2] = BASE64_PADDING_CHAR; \
	(BUFF_OUT)[3] = BASE64_PADDING_CHAR; \
}

#define BASE64_DECODE(BUFF_IN, BUFF_OUT) \
{ \
	(BUFF_OUT)[0] = (((unsigned char)base64_decode_lookup[(unsigned char)(BUFF_IN)[0]]) << 2) | (((unsigned char)base64_decode_lookup[(unsigned char)(BUFF_IN)[1]]) >> 4); \
	(BUFF_OUT)[1] = (((unsigned char)base64_decode_lookup[(unsigned char)(BUFF_IN)[1]]) << 4) | (((unsigned char)base64_decode_lookup[(unsigned char)(BUFF_IN)[2]]) >> 2); \
	(BUFF_OUT)[2] = (((unsigned char)base64_decode_lookup[(unsigned char)(BUFF_IN)[2]]) << 6) | (((unsigned char)base64_decode_lookup[(unsigned char)(BUFF_IN)[3]]) >> 0); \
}

#define BASE64_IS_VALID_BYTE(BYTE) (base64_decode_lookup[(unsigned char)(BYTE)] != NA && (((char)(BYTE)) != BASE64_PADDING_CHAR))

void base64_encode_aligned(const char * restrict input, size_t size, char * restrict output)
{
	size_t i;
	for(i = 0 ; i < (size / 3) ; i++)
	{
		BASE64_ENCODE_3(input, output);
		input += 3, output += 4;
	}
}

void base64_encode(const char * restrict input, size_t size, char * restrict output)
{
	size_t i;
	for(i = 0 ; i < (size / 3) ; i++)
	{
		BASE64_ENCODE_3(input, output);
		input += 3, output += 4;
	}
	if((size % 3) == 2)
	{
		BASE64_ENCODE_2(input, output);
	}
	else if((size % 3) == 1)
	{
		BASE64_ENCODE_1(input, output);
	}
}

int base64_decode(const char * restrict input, size_t size, char * restrict output, size_t *output_size, size_t *error_offset)
{
	if(size == 0)
	{
		*output_size = 0;
	}
	else
	{
		*output_size = BASE64_DECODED_SIZE(size);
		size_t iterations = (size / 4) - 1;
		if((size % 4) != 0)
		{
			return BASE64_ERROR_DECODE_INVALID_SIZE;
		}
		/* Decode 4 byte blocks */
		for(size_t i = 0 ; i < iterations ; i++)
		{
			for(size_t j = 0 ; j < 4 ; j++)
			{
				/* Check for invalid base64 input */
				if(!BASE64_IS_VALID_BYTE(input[j]))
				{
					if(error_offset != NULL) 
					{
						*error_offset = 4*i + j;
					}
					return BASE64_ERROR_DECODE_INVALID_BYTE;
				}
			}
			BASE64_DECODE(input, output);
			input += 4, output += 3;
		}
		/* Decode last 4 bytes */
		for(size_t j = 0 ; j < 4 ; j++)
		{
			/* Check for invalid base64 input */
			if(!BASE64_IS_VALID_BYTE(input[j]))
			{
				if(j > 1 && input[j] == BASE64_PADDING_CHAR) 
				{
					--(*output_size);
				}
				else
				{
					if(error_offset != NULL) 
					{
						*error_offset = 4*iterations + j;
					}
					return BASE64_ERROR_DECODE_INVALID_BYTE;
				}
			}
		}
		/* Check for padding char followed by non-padding char (invalid) */
		if(input[2] == BASE64_PADDING_CHAR && input[3] != BASE64_PADDING_CHAR)
		{
			if(error_offset != NULL) 
			{
				*error_offset = 4*iterations + 3;
			}
			return BASE64_ERROR_DECODE_INVALID_BYTE;
		}
		BASE64_DECODE(input, output);
	}
	return BASE64_OK;
}

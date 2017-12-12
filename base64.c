
#include "base64.h"

static char const *const base64_encode_lookup 
	= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define NA ((char)0xFF)

static char const base64_decode_lookup[] 
	= { NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, 62, NA, NA, NA, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, NA, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
	, NA, NA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, NA, NA, NA, NA
	, NA, NA, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA };

#define BASE64_ENCODE_3(BUFF_IN, BUFF_OUT) \
{ \
	(BUFF_OUT)[0] = base64_encode_lookup[(((BUFF_IN)[0] & 0xFC)>>2)]; \
	(BUFF_OUT)[1] = base64_encode_lookup[((((BUFF_IN)[0] & 0x03)<<4) | (((BUFF_IN)[1] & 0xF0)>>4))]; \
	(BUFF_OUT)[2] = base64_encode_lookup[((((BUFF_IN)[1] & 0x0F)<<2) | (((BUFF_IN)[2] & 0xC0)>>6))]; \
	(BUFF_OUT)[3] = base64_encode_lookup[(((BUFF_IN)[2] & 0x3F)>>0)]; \
}

#define BASE64_ENCODE_2(BUFF_IN, BUFF_OUT) \
{ \
	(BUFF_OUT)[0] = base64_encode_lookup[(((BUFF_IN)[0] & 0xFC)>>2)]; \
	(BUFF_OUT)[1] = base64_encode_lookup[((((BUFF_IN)[0] & 0x03)<<4) | (((BUFF_IN)[1] & 0xF0)>>4))]; \
	(BUFF_OUT)[2] = base64_encode_lookup[(((BUFF_IN)[1] & 0x0F)<<2)]; \
	(BUFF_OUT)[3] = '='; \
}

#define BASE64_ENCODE_1(BUFF_IN, BUFF_OUT) \
{ \
	(BUFF_OUT)[0] = base64_encode_lookup[(((BUFF_IN)[0] & 0xFC)>>2)]; \
	(BUFF_OUT)[1] = base64_encode_lookup[(((BUFF_IN)[0] & 0x03)<<4)]; \
	(BUFF_OUT)[2] = '='; \
	(BUFF_OUT)[3] = '='; \
}

#define BASE64_DECODE(BUFF_IN, BUFF_OUT) \
{ \
	(BUFF_OUT)[0] = (base64_decode_lookup[(BUFF_IN)[0]] << 2) | (base64_decode_lookup[(BUFF_IN)[1]] >> 4); \
	(BUFF_OUT)[1] = (base64_decode_lookup[(BUFF_IN)[1]] << 4) | (base64_decode_lookup[(BUFF_IN)[2]] >> 2); \
	(BUFF_OUT)[2] = (base64_decode_lookup[(BUFF_IN)[2]] << 6) | (base64_decode_lookup[(BUFF_IN)[3]] >> 0); \
}

int base64_encode(FILE *input, FILE *output)
{
	char buffer[BASE64_BUFFER_SIZE];
	char out_bytes[4];
	size_t read;
	size_t encoded;
	size_t to_encode;
	size_t written;
	do
	{
		read = fread(buffer, 1, BASE64_BUFFER_SIZE, input);
		encoded = 0;
		if(read % 3 == 0)
		{
			while(encoded < read)
			{
				BASE64_ENCODE_3(buffer + encoded, out_bytes)
				written = fwrite(out_bytes, 4, 1, output);
				if(written < 1)
					return BASE64_ERROR_WRITING;
				encoded += 3;
			}
		}
		else
		{
			while(encoded < read)
			{
				to_encode = read - encoded;
				if(to_encode < 3)
				{
					if(to_encode == 1)
					{
						BASE64_ENCODE_1(buffer + encoded, out_bytes)
					}
					else /* (to_encode == 2) */
					{
						BASE64_ENCODE_2(buffer + encoded, out_bytes)
					}
					written = fwrite(out_bytes, 4, 1, output);
					if(written < 1)
						return BASE64_ERROR_WRITING;
					encoded = read;
				}
				else
				{
					BASE64_ENCODE_3(buffer + encoded, out_bytes)
					written = fwrite(out_bytes, 4, 1, output);
					if(written < 1)
						return BASE64_ERROR_WRITING;
					encoded += 3;
				}
			}
		}
	} while(read == BASE64_BUFFER_SIZE);
	if(!feof(input))
		return BASE64_ERROR_READING;
	return 0;
}

int base64_decode(FILE *input, FILE *output)
{
	
	return 0;
}

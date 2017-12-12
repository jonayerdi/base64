
#include "base64.h"

char const *const base64_encode_lookup 
	= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define NA ((char)0)

char const base64_decode_lookup[] 
	= { NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, 62, NA, NA, NA, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, NA, NA, NA, NA, NA, NA
	, NA, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, NA, NA, NA, NA, NA
	, NA, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, NA, NA, NA, NA, NA
    , NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA
	, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA, NA };

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
	(BUFF_OUT)[3] = '='; \
}

#define BASE64_ENCODE_1(BUFF_IN, BUFF_OUT) \
{ \
	(BUFF_OUT)[0] = base64_encode_lookup[(unsigned char)(((BUFF_IN)[0] & 0xFC)>>2)]; \
	(BUFF_OUT)[1] = base64_encode_lookup[(unsigned char)(((BUFF_IN)[0] & 0x03)<<4)]; \
	(BUFF_OUT)[2] = '='; \
	(BUFF_OUT)[3] = '='; \
}

#define BASE64_DECODE(BUFF_IN, BUFF_OUT) \
{ \
	(BUFF_OUT)[0] = (base64_decode_lookup[(unsigned char)(BUFF_IN)[0]] << 2) | (base64_decode_lookup[(unsigned char)(BUFF_IN)[1]] >> 4); \
	(BUFF_OUT)[1] = (base64_decode_lookup[(unsigned char)(BUFF_IN)[1]] << 4) | (base64_decode_lookup[(unsigned char)(BUFF_IN)[2]] >> 2); \
	(BUFF_OUT)[2] = (base64_decode_lookup[(unsigned char)(BUFF_IN)[2]] << 6) | (base64_decode_lookup[(unsigned char)(BUFF_IN)[3]] >> 0); \
}

inline int base64_encode(const char *input, size_t size, char *output)
{
	size_t i;
	for(i = 0 ; i < (size / 3) ; i++)
	{
		BASE64_ENCODE_3(input + (3 * i), output + (4 * i));
	}
	if((size % 3) == 2)
	{
		BASE64_ENCODE_2(input + (3 * i), output + (4 * i));
	}
	else if((size % 3) == 1)
	{
		BASE64_ENCODE_1(input + (3 * i), output + (4 * i));
	}
	return 0;
}

inline int base64_decode(const char *input, size_t size, char *output, size_t *output_size)
{
	if(size > 0)
	{
		if((size % 4) != 0)
		{
			return BASE64_ERROR_DECODE_SIZE;
		}
		for(size_t i = 0 ; i < (size / 4) ; i++)
		{
			BASE64_DECODE(input + (4 * i), output + (3 * i));
		}
		*output_size = ((input[size - 1] == '=') ? ((input[size - 2] == '=') ? (BASE64_DECODED_SIZE(size) - 2) : (BASE64_DECODED_SIZE(size) - 1) ) : (BASE64_DECODED_SIZE(size)));
	}
	return 0;
}

int base64_encode_file(FILE *input, FILE *output)
{
	char buffer[BASE64_ENCODE_BUFFER_SIZE];
	char out_bytes[4];
	size_t read;
	size_t encoded;
	size_t to_encode;
	size_t written;
	do
	{
		read = fread(buffer, 1, BASE64_ENCODE_BUFFER_SIZE, input);
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
	} while(read == BASE64_ENCODE_BUFFER_SIZE);
	if(!feof(input))
		return BASE64_ERROR_READING;
	return 0;
}

int base64_decode_file(FILE *input, FILE *output)
{
	char buffer_in[BASE64_DECODE_BUFFER_SIZE];
	char buffer_out[BASE64_DECODED_SIZE(BASE64_DECODE_BUFFER_SIZE)];
	size_t read;
	size_t decoded;
	size_t written;
	do
	{
		read = fread(buffer_in, 1, BASE64_DECODE_BUFFER_SIZE, input);
		base64_decode(buffer_in, read, buffer_out, &decoded);
		written = fwrite(buffer_out, decoded, 1, output);
		if(written < 1)
			return BASE64_ERROR_WRITING;
	} while(read == BASE64_DECODE_BUFFER_SIZE);
	if(!feof(input))
		return BASE64_ERROR_READING;
	return 0;
}

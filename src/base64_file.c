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

#include "base64_file.h"

int base64_encode_file(FILE *input, FILE *output)
{
	char buffer_in[BASE64_ENCODE_BUFFER_SIZE];
	char buffer_out[BASE64_ENCODED_SIZE(BASE64_ENCODE_BUFFER_SIZE)];
	size_t read;
	size_t encoded;
	size_t written;
	do
	{
		read = fread(buffer_in, 1, BASE64_ENCODE_BUFFER_SIZE, input);
		if(read == BASE64_ENCODE_BUFFER_SIZE)
		{
			base64_encode_aligned(buffer_in, read, buffer_out);
			encoded = BASE64_ENCODED_SIZE(BASE64_ENCODE_BUFFER_SIZE);
			written = fwrite(buffer_out, encoded, 1, output);
			if(!written && encoded)
				return BASE64_ERROR_WRITING;
		}
		else
		{
			base64_encode(buffer_in, read, buffer_out);
			encoded = BASE64_ENCODED_SIZE(read);
			written = fwrite(buffer_out, encoded, 1, output);
			if(!written && encoded)
				return BASE64_ERROR_WRITING;
			break;
		}
	} while(1);
	if(!feof(input))
		return BASE64_ERROR_READING;
	return BASE64_OK;
}

int base64_decode_file(FILE *input, FILE *output, size_t *error_offset)
{
	int error;
	char buffer_in[BASE64_DECODE_BUFFER_SIZE];
	char buffer_out[BASE64_DECODED_SIZE(BASE64_DECODE_BUFFER_SIZE)];
	size_t iterations = 0;
	size_t read;
	size_t decoded;
	size_t written;
	do
	{
		read = fread(buffer_in, 1, BASE64_DECODE_BUFFER_SIZE, input);
		error = base64_decode(buffer_in, read, buffer_out, &decoded, error_offset);
		if(error)
		{
			if(error_offset != NULL)
			{
				*error_offset += iterations * BASE64_DECODE_BUFFER_SIZE;
			}
			return error;
		}
		written = fwrite(buffer_out, decoded, 1, output);
		if(!written && decoded)
			return BASE64_ERROR_WRITING;
		iterations++;
	} while(read == BASE64_DECODE_BUFFER_SIZE);
	if(!feof(input))
		return BASE64_ERROR_READING;
	return BASE64_OK;
}

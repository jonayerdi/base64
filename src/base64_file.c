
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
		base64_encode(buffer_in, read, buffer_out);
		encoded = BASE64_ENCODED_SIZE(read);
		written = fwrite(buffer_out, encoded, 1, output);
		if(written < 1 && encoded > 0)
			return BASE64_ERROR_WRITING;
	} while(read == BASE64_ENCODE_BUFFER_SIZE);
	if(!feof(input))
		return BASE64_ERROR_READING;
	return BASE64_OK;
}

int base64_decode_file(FILE *input, FILE *output)
{
	int error;
	char buffer_in[BASE64_DECODE_BUFFER_SIZE];
	char buffer_out[BASE64_DECODED_SIZE(BASE64_DECODE_BUFFER_SIZE)];
	size_t read;
	size_t decoded;
	size_t written;
	do
	{
		read = fread(buffer_in, 1, BASE64_DECODE_BUFFER_SIZE, input);
		error = base64_decode(buffer_in, read, buffer_out, &decoded, NULL);
		if(error)
			return error;
		written = fwrite(buffer_out, decoded, 1, output);
		if(written < 1 && decoded > 0)
			return BASE64_ERROR_WRITING;
	} while(read == BASE64_DECODE_BUFFER_SIZE);
	if(!feof(input))
		return BASE64_ERROR_READING;
	return BASE64_OK;
}

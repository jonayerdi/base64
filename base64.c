
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define BASE64_MODE_NULL 0
#define BASE64_MODE_ENCODE 1
#define BASE64_MODE_DECODE 2
#define BASE64_MODE_STDIN 4
#define BASE64_MODE_STDOUT 8

#define BASE64_ERROR_ARGS -1
#define BASE64_ERROR_OPEN_INPUT -2
#define BASE64_ERROR_OUTPUT_EXISTS -3
#define BASE64_ERROR_OPEN_OUTPUT -4
#define BASE64_ERROR_PARSE_ARG1 -5
#define BASE64_ERROR_READING -6
#define BASE64_ERROR_WRITING -7
#define BASE64_ERROR_DECODE -8

/* !! Must be a multiple of 3 !! */
#define BASE64_BUFFER_SIZE 3072

static char const *const base64_encode_lookup 
	= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//static char const *const base64_decode_lookup = {};

#define BASE64_CLOSE_STREAMS(ISTREAM, OSTREAM, MODE) \
{ \
	if((!((MODE) & BASE64_MODE_STDIN) && ((ISTREAM) != NULL))) \
		fclose(ISTREAM); \
	if((!((MODE) & BASE64_MODE_STDOUT) && ((OSTREAM) != NULL))) \
		fclose(OSTREAM); \
}

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

int base64_encode(FILE *input, FILE *output);
int base64_decode(FILE *input, FILE *output);

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

int main(int argc, char *argv[])
{
	/* Declare variables */
	FILE *input = NULL;
	FILE *output = NULL;
	char mode = BASE64_MODE_NULL;
	/* Check args count */
	if(argc != 4)
	{
		printf("Usage:\n");
		printf("base64 <e|encode|d|decode> <file_in> <file_out>\n");
		printf("If <file_in> is set to * the program reads from stdin\n");
		printf("If <file_out> is set to * the program writes to stdout\n");
		return BASE64_ERROR_ARGS;
	}
	/* Parse args and run program */
	else
	{
		/* Parse arg 1 -> BASE64_MODE_ENCODE or BASE64_MODE_DECODE */
		if(!strcmp(argv[1], "e") || !strcmp(argv[1], "encode"))
			mode = BASE64_MODE_ENCODE;
		else if(!strcmp(argv[1], "d") || !strcmp(argv[1], "decode"))
			mode = BASE64_MODE_DECODE;
		else
		{
			printf("Error: first argument must be one of these:\ne\nencode\nd\ndecode\n");
			return BASE64_ERROR_PARSE_ARG1;
		}
		/* Parse arg 2 -> Input stream */
		if(!strcmp(argv[2], "*"))
		{
			input = stdin;
			mode |= BASE64_MODE_STDIN;
		}
		else
		{
			input = fopen(argv[2], "rb");
			if(input == NULL)
			{
				printf("Error opening input file %s\n", argv[2]);
				return BASE64_ERROR_OPEN_INPUT;
			}
		}
		/* Parse arg 3 -> Output stream */
		if(!strcmp(argv[3], "*"))
		{
			output = stdout;
			mode |= BASE64_MODE_STDOUT;
		}
		else
		{
			output = fopen(argv[3], "rb");
			if(output != NULL)
			{
				BASE64_CLOSE_STREAMS(input, output, mode);
				printf("Error: output file %s already exists\n", argv[3]);
				return BASE64_ERROR_OUTPUT_EXISTS;
			}
			output = fopen(argv[3], "wb");
			if(output == NULL)
			{
				BASE64_CLOSE_STREAMS(input, output, mode);
				printf("Error opening output file %s\n", argv[3]);
				return BASE64_ERROR_OPEN_OUTPUT;
			}
		}
		/* Encode or decode input stream into output stream */
		int retval = BASE64_ERROR_PARSE_ARG1;
		if(mode & BASE64_MODE_ENCODE)
			retval = base64_encode(input, output);
		else if(mode == BASE64_MODE_DECODE)
			retval = base64_decode(input, output);
		/* Close streams and return */
		BASE64_CLOSE_STREAMS(input, output, mode);
		return retval;
	}
}

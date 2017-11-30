
#include <stdio.h>
#include <string.h>

#define BASE64_MODE_NULL 0
#define BASE64_MODE_ENCODE 1
#define BASE64_MODE_DECODE 2

#define BASE64_ERROR_ARGS -1
#define BASE64_ERROR_OPEN_INPUT -2
#define BASE64_ERROR_OUTPUT_EXISTS -3
#define BASE64_ERROR_OPEN_OUTPUT -4
#define BASE64_ERROR_PARSE_ARG1 -5
#define BASE64_ERROR_READING -6
#define BASE64_ERROR_WRITING -7
#define BASE64_ERROR_DECODE -8

#define BASE64_BUFFER_SIZE 3072

static char const *const base64_encode_lookup 
	= "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//static char const *const base64_decode_lookup = {};

int base64_encode(FILE *input, FILE *output);
int base64_decode(FILE *input, FILE *output);

int base64_encode(FILE *input, FILE *output)
{
	
	return 0;
}

int base64_decode(FILE *input, FILE *output)
{
	
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		printf("Usage:\n");
		printf("base64 <e|encode|d|decode> <file_in> <file_out>\n");
		return BASE64_ERROR_ARGS;
	}
	else
	{
		char mode = BASE64_MODE_NULL;
		if(strcmp(argv[1], "e") || strcmp(argv[1], "encode"))
			mode = BASE64_MODE_ENCODE;
		else if(strcmp(argv[1], "d") || strcmp(argv[1], "decode"))
			mode = BASE64_MODE_DECODE;
		else
		{
			printf("Error: first argument must be one of these:\ne\nencode\nd\ndecode>\n");
			return BASE64_ERROR_PARSE_ARG1;
		}
		FILE *input = fopen(argv[2], "rb");
		if(input == NULL)
		{
			printf("Error opening input file %s\n", argv[2]);
			return BASE64_ERROR_OPEN_INPUT;
		}
		FILE *output;
		output = fopen(argv[3], "rb");
		if(output != NULL)
		{
			fclose(output);
			printf("Error: output file %s already exists\n", argv[3]);
			return BASE64_ERROR_OUTPUT_EXISTS;
		}
		output = fopen(argv[3], "wb");
		if(output == NULL)
		{
			printf("Error opening output file %s\n", argv[3]);
			return BASE64_ERROR_OPEN_OUTPUT;
		}
		int retval = BASE64_ERROR_PARSE_ARG1;
		if(mode == BASE64_MODE_ENCODE)
			retval = base64_encode(input, output);
		else if(mode == BASE64_MODE_DECODE)
			retval = base64_decode(input, output);
		fclose(input);
		fclose(output);
		return retval;
	}
	return 0;
}

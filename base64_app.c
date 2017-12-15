
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "base64.h"

#define BASE64_MODE_NULL 0
#define BASE64_MODE_ENCODE 1
#define BASE64_MODE_DECODE 2
#define BASE64_MODE_STDIN 4
#define BASE64_MODE_STDOUT 8

#define BASE64_ERROR_ARGS 1
#define BASE64_ERROR_OPEN_INPUT 2
#define BASE64_ERROR_OUTPUT_EXISTS 3
#define BASE64_ERROR_OPEN_OUTPUT 4
#define BASE64_ERROR_PARSE_ARG1 5

#define BASE64_CLOSE_STREAMS(ISTREAM, OSTREAM, MODE) \
{ \
	if((!((MODE) & BASE64_MODE_STDIN) && ((ISTREAM) != NULL))) \
		fclose(ISTREAM); \
	if((!((MODE) & BASE64_MODE_STDOUT) && ((OSTREAM) != NULL))) \
		fclose(OSTREAM); \
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
		printf("base64 e|encode|d|decode <file_in> <file_out>\n");
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
			retval = base64_encode_file(input, output);
		else if(mode & BASE64_MODE_DECODE)
			retval = base64_decode_file(input, output);
		/* Close streams and return */
		BASE64_CLOSE_STREAMS(input, output, mode);
		return retval;
	}
}

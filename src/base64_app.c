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

#include <stddef.h>
#include <string.h>
#include <stdio.h>

#include "base64.h"
#include "base64_file.h"

/* This is to fix newline conversion for stdin and stdout */
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

#define BASE64_STDIO_FILE "."

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
	int retval;
	size_t error_offset;
	FILE *input = NULL;
	FILE *output = NULL;
	char mode = BASE64_MODE_NULL;
	/* Check args count */
	if(argc != 4)
	{
		printf("Usage:\n");
		printf("base64 e|encode|d|decode <file_in> <file_out>\n");
		printf("If <file_in> is set to " BASE64_STDIO_FILE " the program reads from stdin\n");
		printf("If <file_out> is set to " BASE64_STDIO_FILE " the program writes to stdout\n");
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
			fprintf(stderr, "Error: first argument must be one of these:\ne\nencode\nd\ndecode\n");
			return BASE64_ERROR_PARSE_ARG1;
		}
		/* Parse arg 2 -> Input stream */
		if(!strcmp(argv[2], BASE64_STDIO_FILE))
		{
			/* This is to fix newline conversion for stdin and stdout */
			#ifdef _WIN32
			_setmode(_fileno(stdin), O_BINARY);
			#endif
			input = stdin;
			mode |= BASE64_MODE_STDIN;
		}
		else
		{
			input = fopen(argv[2], "rb");
			if(input == NULL)
			{
				fprintf(stderr, "Error opening input file %s\n", argv[2]);
				return BASE64_ERROR_OPEN_INPUT;
			}
		}
		/* Parse arg 3 -> Output stream */
		if(!strcmp(argv[3], BASE64_STDIO_FILE))
		{
			/* This is to fix newline conversion for stdin and stdout */
			#ifdef _WIN32
			_setmode(_fileno(stdout), O_BINARY);
			#endif
			output = stdout;
			mode |= BASE64_MODE_STDOUT;
		}
		else
		{
			output = fopen(argv[3], "rb");
			if(output != NULL)
			{
				BASE64_CLOSE_STREAMS(input, output, mode);
				fprintf(stderr, "Error: output file %s already exists\n", argv[3]);
				return BASE64_ERROR_OUTPUT_EXISTS;
			}
			output = fopen(argv[3], "wb");
			if(output == NULL)
			{
				BASE64_CLOSE_STREAMS(input, output, mode);
				fprintf(stderr, "Error opening output file %s\n", argv[3]);
				return BASE64_ERROR_OPEN_OUTPUT;
			}
		}
		/* Encode or decode input stream into output stream */
		if(mode & BASE64_MODE_ENCODE)
			retval = base64_encode_file(input, output);
		else
			retval = base64_decode_file(input, output, &error_offset);
		/* Print errors if necessary */
		switch(retval)
		{
			case BASE64_OK:
				/* OK */
				break;
			case BASE64_ERROR_READING:
				fprintf(stderr, "Error reading from input file\n");
				break;
			case BASE64_ERROR_WRITING:
				fprintf(stderr, "Error writing to output file\n");
				break;
			case BASE64_ERROR_DECODE_INVALID_BYTE:
				fprintf(stderr, "Error decoding: Invalid input character at offset %u\n", (unsigned)error_offset);
				break;
			case BASE64_ERROR_DECODE_INVALID_SIZE:
				fprintf(stderr, "Error decoding: Invalid input length\n");
				break;
			default:
				fprintf(stderr, "Unknown error code: %d\n", retval);
				break;
		}
		/* Close streams and return */
		BASE64_CLOSE_STREAMS(input, output, mode);
		return retval;
	}
}

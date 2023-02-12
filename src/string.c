#define STRCMP_PREFIX(s1, prefix) strncmp(s1, prefix, strlen(prefix))

#define FILE_BUFFER_CHUNK_SIZE 256
/* returns the last char read, which is always something in .ifs or EOF */
/* .buffer is allowed to be NULL */
static int read_file_until_ifs(char **buffer, FILE *fp, const char *ifs)
{
	size_t ptr = 0;
	size_t ifsptr;
	int byte = 0;
	if (buffer) *buffer = malloc(FILE_BUFFER_CHUNK_SIZE);
	while (1)
	{
		byte = fgetc(fp);
		for (ifsptr = 0; ifsptr < strlen(ifs); ifsptr++)
			if (byte == ifs[ifsptr])
				goto read_file_until_ifs_eof;

		if (byte == EOF)
		{
read_file_until_ifs_eof:
			if (buffer) (*buffer)[ptr] = '\0';
			break;
		}

		if (buffer)
		{
			(*buffer)[ptr] = byte;
			ptr++;
			if (!ptr%FILE_BUFFER_CHUNK_SIZE)
				*buffer = realloc(*buffer, ptr+FILE_BUFFER_CHUNK_SIZE); /* allocate another chunk if needed */
		}
	}
	if (buffer) *buffer = realloc(*buffer, strlen(*buffer)+1);
	return byte;
}

static int read_file_until_string(FILE *fp, const char *end)
{
	size_t ptr = 0;
	int byte = 0;
	while (1)
	{
		byte = fgetc(fp);
		if (byte == EOF)
			return EOF;

		if (byte == end[ptr])
			ptr++;
		else
			ptr = 0;

		if (ptr == strlen(end))
			return 0;
	}
}

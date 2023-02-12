void liberty_free_dialogue(LibertyDialogue *d)
{
	if (!d) return;

	if (d->name) free(d->name);
	if (d->text) free(d->text);

	for (uint8_t i = 0; i < d->optionc; i++)
		if (d->options[i])
			free(d->options[i]);
	free(d);
}

/* http://www.cse.yorku.ca/~oz/hash.html */
uint32_t djb2(char *str)
{
	uint32_t hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash<<5) + hash) + c; /* hash * 33 + c */

	return hash;
}

typedef struct DialogueFile
{
	uint32_t hash;
	FILE    *fp;
} DialogueFile;

typedef struct DialogueFiles
{ /* alloc(sizeof(DialogueFiles) + sizeof(DialogueFile)*count) */
	size_t       count;
	FILE        *activefp;
	DialogueFile file[];
} DialogueFiles;
DialogueFiles *df; /* pretty sure this is fine to be a global */

static void free_dialogue_memoization(void)
{
	if (!df) return;
	for (size_t i = 0; i < df->count; i++)
		fclose(df->file[i].fp);
	free(df); df = NULL;
}

static void goto_dialogue_tag(char *tag)
{
	if (!tag) return; /* nothing to do */

	char *buffer;
	int tailchar;
	while (1)
	{
		tailchar = read_file_until_ifs(&buffer, df->activefp, "\n");
		if (tailchar == EOF) { free(buffer); return; }

		if (!STRCMP_PREFIX(buffer, "tag"))
		{
			if (!strcmp(strchr(buffer, ' ')+1, tag))
			{
				free(buffer);
				return;
			}
		}
		free(buffer);
	}
}

LibertyDialogue *liberty_get_next_dialogue(void)
{
	LibertyDialogue *d = calloc(1, sizeof(LibertyDialogue));

	char *buffer;
	int tailchar;
	size_t textsize = 0;
	char *text = NULL;
	while (1)
	{
		tailchar = read_file_until_ifs(&buffer, df->activefp, "\n");

		/* empty line or EOF, block terminator */
		if (tailchar == EOF || !strlen(buffer))
		{
			free(buffer);

			/* allow blocks to be terminated by multiple empty lines */
			if (text)
				d->text = text;
			else if (tailchar == EOF)
			{
				free(d);
				return NULL;
			} else
				continue;

			break;
		}

		/* discard comments */
		if (buffer[0] == '#')
		{
			free(buffer);
			continue;
		}

		/* tag */
		if (!STRCMP_PREFIX(buffer, "tag"))
		{
			free(buffer);
			continue;
		}

		/* goto */
		if (!STRCMP_PREFIX(buffer, "goto"))
		{
			rewind(df->activefp);
			goto_dialogue_tag(strchr(buffer, ' ')+1);

			free(buffer);
			continue;
		}

		/* close */
		if (!STRCMP_PREFIX(buffer, "close"))
		{
			free(buffer);
			free(d);
			return NULL;
		}

		/* line starts with tab, it's part of d->text */
		if (buffer[0] == '\t')
		{
			size_t oldtextsize = textsize;
			textsize += strlen(buffer); /* -1 for leading tab, +1 for either the terminating nul byte or a newline */
			text = realloc(text, textsize);

			if (oldtextsize)
			{
				memcpy(&text[oldtextsize], buffer + 1, strlen(buffer));
				text[oldtextsize - 1] = '\n';
			} else
			{
				memcpy(&text[oldtextsize], buffer + 1, strlen(buffer));
			}

			free(buffer);
			continue;
		}

		/* line contains a colon, it's the name */
		if (strchr(buffer, ':'))
		{
			buffer[strchr(buffer, ':') - buffer] = '\0';
			d->name = strdup(buffer);

			free(buffer);
			continue;
		}


		/* fallback case */
		free(buffer);
	}
	return d;
}

/* tag can be NULL to start the file from the beginning */
void liberty_goto_dialogue_file_tag(char *path, char *tag)
{
	if (!df) df = calloc(1, sizeof(DialogueFiles));

	uint32_t hash = djb2(path);
	for (size_t i = 0; i < df->count; i++)
		if (df->file[i].hash == hash)
		{
			df->activefp = df->file[i].fp;
			goto liberty_read_dialogue_file_readfile;
		}
	/* the filepointer isn't memoized yet, memoize it */
	df = realloc(df, sizeof(DialogueFiles) + sizeof(DialogueFile)*(df->count+1));
	df->file[df->count].hash = hash;
	df->file[df->count].fp = df->activefp = fopen(path, "r");
	df->count++;

liberty_read_dialogue_file_readfile:
	/* return to the top of the file */
	rewind(df->activefp);

	goto_dialogue_tag(tag);
}

/* no draw code is provided, write your own */

typedef struct LibertyDialogue
{
	char   *name;    /* speaker of the dialogue */
	char   *text;    /* body of the dialogue */
	/* TODO: the below values are for branching that isn't yet defined, so ignore them for the time being */
	uint8_t option;  /* 0x0 through 0xf is allowed */
	uint8_t optionc; /* how many options are used */
	char   *options[0xf];
} LibertyDialogue;

void liberty_free_dialogue(LibertyDialogue *d);

/* SCRIPT FILE FORMAT:
 *   scripting language for liberty dialogue
 *
 * keywords:
 * tag {tagname}  ->                            drop a tag that can be jumped to
 * goto {tagname} ->                            jump to a tag, can jump forwards or backwards
 * close          ->                            close the dialogue box
 * #              -> line starting with hash:   comment
 * {speaker}:     -> line ending with colon:    start dialogue entry
 * \t{textline}:  -> line starting with a tab:  a line of text
 *                -> empty line:                end dialogue entry
 */

/* load file .path as a script (files are memoized) and jump to tag .tag */
void liberty_goto_dialogue_file_tag(char *path, char *tag);

/* return the next dialogue entry or NULL for EOF/close */
LibertyDialogue *liberty_get_next_dialogue(void);

/* callback returns 1 to continue to the next line and 2 to break */
void liberty_add_custom_dialogue_function(int (*callback)(const char *line));
void liberty_clear_custom_dialogue_functions(void);

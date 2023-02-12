/* no draw code is provided, write your own */

typedef struct LibertyDialogue
{
	char   *name; /* NULL if the dialogue is parentless */
	char   *text;
	uint8_t option; /* 0x0 through 0xf is allowed */
	uint8_t optionc; /* how many options are used */
	char   *options[0xf];
} LibertyDialogue;

void liberty_free_dialogue(LibertyDialogue *d);

void liberty_goto_dialogue_file_tag(char *path, char *tag);
LibertyDialogue *liberty_get_next_dialogue(void);

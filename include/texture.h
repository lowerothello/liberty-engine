/* spritesheet */
typedef struct LibertyTexture
{
	uint32_t       w, h;
	void          *image; /* private */
	uint32_t       spritecount;
	LibertyIntRect sprite[];
} LibertyTexture;

LibertyTexture *liberty_new_texture_from_file(const char *path);
void liberty_free_texture(LibertyTexture *texture);

/* returns the new sprite index */
/* reallocs texture internally */
uint32_t liberty_add_texture_sprite(LibertyTexture **texture, LibertyIntRect sprite);

void liberty_draw_texture(LibertyTexture *texture, LibertyVec2 pos, float rotate, bool hflip, bool vflip);
void liberty_draw_texture_sprite(LibertyTexture *texture, uint32_t sprite, LibertyVec2 pos, float rotate, bool hflip, bool vflip);

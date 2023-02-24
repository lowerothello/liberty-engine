typedef struct Raindrop
{
	LibertyRGB colour;
	float      speed;
	float      yoffset;
} Raindrop;

void drawRaindrop(LibertyEntity *entity, LibertyVec2 camera)
{
	Raindrop *raindrop = entity->data;
	liberty_set_colour(raindrop->colour);
	liberty_draw_line(
			(LibertyVec2){entity->collision.x - camera.x, entity->collision.y - camera.y + raindrop->yoffset},
			(LibertyVec2){entity->collision.x - camera.x, entity->collision.y - camera.y + raindrop->yoffset + entity->collision.h});

	char buffer[32];
	snprintf(buffer, 32, "raindrop: %d x %d", (int)entity->collision.x, (int)(entity->collision.y + raindrop->yoffset));
	liberty_draw_font_string(font, (LibertyVec2){WIDTH - 80, 70}, buffer);
}

#define RAINDROP_ENTITY_TYPE 2
#define RAINDROP_SPEED_RANGE 300
#define RAINDROP_SPEED_MIN 700
#define RAINDROP_MAX_HEIGHT 30
#define RAINDROP_SATURATION 0x28 /* (uint8_t) */
#define RAINDROP_VALUE      0xc0 /* (uint8_t) */
#define RAINDROP_ALPHA      0x40 /* (uint8_t) */
void resetRaindrop(LibertyEntity *entity)
{
	Raindrop *raindrop = entity->data;

	raindrop->yoffset = 0;
	raindrop->colour = liberty_hsv_to_rgb((LibertyHSV){rand()%256, RAINDROP_SATURATION, RAINDROP_VALUE, RAINDROP_ALPHA});
	raindrop->speed = RAINDROP_SPEED_MIN + rand()%RAINDROP_SPEED_RANGE;
	entity->collision.x = p.camera.x - (WIDTH>>1) + rand()%(WIDTH<<1);
	entity->collision.y = p.camera.y - (HEIGHT>>1);
	entity->collision.h = 1 + rand()%(RAINDROP_MAX_HEIGHT-1);
}

void updateRaindrop(LibertyEntity *entity, float deltatime)
{
	Raindrop *raindrop = entity->data;

	raindrop->yoffset += raindrop->speed*deltatime;
	if (raindrop->yoffset > (HEIGHT<<1))
		resetRaindrop(entity);
}

void spawnRaindrop(void)
{
	resetRaindrop(liberty_add_entity(RAINDROP_ENTITY_TYPE,
				(LibertyRect){0, 0, 0, 0},
				NULL, sizeof(Raindrop),
				updateRaindrop, drawRaindrop));
}

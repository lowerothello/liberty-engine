#include "../include/liberty.h"

#define WIDTH 320
#define HEIGHT 180
#define NOISE_TEX_SIZE 1024

LibertyConfig liberty_default_config =
{
	.title = "Liberty",
	.width = WIDTH,
	.height = HEIGHT,
	.scale = 3,
	.fullscreen = 0,
	.vsync = 0,
	.framerate = 0.0f,
};

LibertyFont     *font, *dialogueprompt;
LibertyLayer    *uiLayer, *noiseLayer;

LibertyDialogue *dialogue;
float dialogue_visiblechars = 0.0f;
#define DIALOGUE_TEXT_SPEED 30

typedef struct TestEnitityState
{
	float hue;
} TestEntityState;

void updateTestEntity(LibertyEntity *entity, float deltatime)
{
	((TestEntityState*)entity->data)->hue = fmodf(((TestEntityState*)entity->data)->hue + 1.0f*deltatime, 1.0f);
	((TestEntityState*)entity->data)->hue = fmodf(((TestEntityState*)entity->data)->hue, 1.0f);
}
void drawTestEntity(LibertyEntity *entity, LibertyVec2 camera)
{
	LibertyRGB entitydrawcolour = liberty_hsv_to_rgb((LibertyHSV){((TestEntityState*)entity->data)->hue*0xff, 0x80, 0xff, 0x80});
	liberty_set_colour(entitydrawcolour);
	liberty_draw_rect(1,
			(LibertyRect){
				entity->collision.x - camera.x,
				entity->collision.y - camera.y,
				entity->collision.w,
				entity->collision.h
			});
	char buffer[8];
	snprintf(buffer, 10, "#%02x%02x%02x", entitydrawcolour.r, entitydrawcolour.g, entitydrawcolour.b);
	LIBERTY_DRAW_FONT_STRING_REVERSE_CENTRE(font,
			((LibertyRect){
				entity->collision.x - camera.x,
				entity->collision.y - camera.y + entity->collision.w + 2,
				entity->collision.w,
				entity->collision.h
			}),
			buffer);
}


void liberty_callback_init(void)
{
	font = liberty_new_font_from_file("Untitled1-6.bdf");
	dialogueprompt = liberty_new_font_from_file("dialogueprompt-8.bdf");
	uiLayer = liberty_new_layer((LibertyVec2){WIDTH, HEIGHT});
	noiseLayer = liberty_new_layer((LibertyVec2){NOISE_TEX_SIZE, NOISE_TEX_SIZE});
	liberty_set_draw_layer(noiseLayer);
	liberty_set_colour((LibertyRGB){0x00, 0x00, 0x00, 0xff});
	liberty_draw_clear();

	for (uint16_t x = 0; x < NOISE_TEX_SIZE; x++)
		for (uint16_t y = 0; y < NOISE_TEX_SIZE; y++)
		{
			liberty_set_colour((LibertyRGB){0xff, 0xff, 0xff, rand()%256});
			liberty_draw_pixel((LibertyVec2){x, y});
		}
	liberty_set_draw_layer(NULL);

	liberty_goto_dialogue_file_tag("dialoguetest", NULL);

	dialogue = liberty_get_next_dialogue();

	TestEntityState entitystate;
	entitystate.hue = 0.0f;
	liberty_add_entity(1, (LibertyRect){20, 20, 8, 8}, &entitystate, sizeof(TestEntityState),
			updateTestEntity, drawTestEntity);
}

void liberty_callback_cleanup(void)
{
	liberty_free_font(font);
	liberty_free_font(dialogueprompt);
	liberty_free_layer(uiLayer);
	liberty_free_layer(noiseLayer);
	liberty_free_dialogue(dialogue);
}


#define PLAYER_SIZE 0x10
typedef struct Player
{
	LibertyVec2 pos;
	LibertyVec2 accel;
	LibertyVec2 camera;

	struct {
		bool up;
		bool down;
		bool left;
		bool right;
	} input;
} Player;
Player p;

#define WALK_ACCEL 2000.f
#define WALK_FRICTION 20.f
#define WALK_TERMINAL 100.f

void playerInput(SDL_Event event)
{
	if (event.key.repeat) return;
	switch (event.key.keysym.sym)
	{
		case SDLK_UP:    p.input.up =    (event.type == SDL_EVENT_KEY_UP) ? 0 : 1; break;
		case SDLK_DOWN:  p.input.down =  (event.type == SDL_EVENT_KEY_UP) ? 0 : 1; break;
		case SDLK_LEFT:  p.input.left =  (event.type == SDL_EVENT_KEY_UP) ? 0 : 1; break;
		case SDLK_RIGHT: p.input.right = (event.type == SDL_EVENT_KEY_UP) ? 0 : 1; break;

		case SDLK_SPACE:
			if (event.type == SDL_EVENT_KEY_DOWN)
			{
				liberty_free_dialogue(dialogue);
				dialogue = liberty_get_next_dialogue();
				dialogue_visiblechars = 0.0f;
			}
			break;
	}
}

LibertySignal liberty_callback_event(SDL_Event event)
{
	switch (event.type)
	{
		case SDL_EVENT_KEY_DOWN: /* fall through */
		case SDL_EVENT_KEY_UP:
			playerInput(event);
			break;
	}
	return LIBERTY_SIGNAL_OK;
}

void handleAccel(double deltatime,
		LibertyVec2 *accel, LibertyVec2 joystick,
		float coef_speed, float coef_friction, float coef_max)
{
	if (fabsf(accel->x) < coef_max) accel->x += joystick.x * coef_speed*deltatime;
	if (fabsf(accel->y) < coef_max) accel->y += joystick.y * coef_speed*deltatime;

	accel->x = LERP(accel->x, 0, coef_friction*deltatime);
	accel->y = LERP(accel->y, 0, coef_friction*deltatime);
}

#define CAMERA_SPEED 20

LibertySignal liberty_callback_update(double deltatime)
{

	handleAccel(deltatime, &p.accel,
			(LibertyVec2){p.input.right - p.input.left, p.input.down - p.input.up},
			WALK_ACCEL, WALK_FRICTION, WALK_TERMINAL);

	p.pos.x += p.accel.x*deltatime;
	p.pos.y += p.accel.y*deltatime;

	LibertyVec2 cameratarget;
	cameratarget.x = p.pos.x - ((WIDTH - PLAYER_SIZE)>>1);
	cameratarget.y = p.pos.y - ((HEIGHT - PLAYER_SIZE)>>1);

	/* allows overshoot, not sure if that's a problem */
	p.camera.x = LERP(p.camera.x, cameratarget.x, CAMERA_SPEED*deltatime);
	p.camera.y = LERP(p.camera.y, cameratarget.y, CAMERA_SPEED*deltatime);

	liberty_update_entities(deltatime);

	return LIBERTY_SIGNAL_OK;
}

#define DIALOGUE_WIDTH 200
#define DIALOGUE_HEIGHT 50
#define DIALOGUE_WIDTH_MARGIN 11
#define DIALOGUE_HEIGHT_MARGIN 13
void draw_dialogue(LibertyDialogue *d, double deltatime)
{
	if (!d) return;

	float x = ((WIDTH - DIALOGUE_WIDTH)>>1);
	float y = (HEIGHT>>1) + (((HEIGHT>>1) - DIALOGUE_HEIGHT)>>1);
	liberty_draw_rect(0, (LibertyRect){x, y, DIALOGUE_WIDTH, DIALOGUE_HEIGHT});

	if (d->name && strlen(d->name))
		LIBERTY_DRAW_FONT_STRING_REVERSE_CENTRE(font, ((LibertyRect){x, y+1, DIALOGUE_WIDTH, 0}), d->name);

	if (d->text)
	{
		if (dialogue_visiblechars < strlen(d->text))
			dialogue_visiblechars += DIALOGUE_TEXT_SPEED*deltatime;
		else
			liberty_draw_font_animation(dialogueprompt,
					(LibertyVec2){x + DIALOGUE_WIDTH - DIALOGUE_WIDTH_MARGIN, y + DIALOGUE_HEIGHT - DIALOGUE_HEIGHT_MARGIN},
					10*deltatime);

		LIBERTY_DRAW_FONT_STRING_COUNT_CENTRE(font, ((LibertyRect){x, y, DIALOGUE_WIDTH, DIALOGUE_HEIGHT}), d->text, dialogue_visiblechars);
	}
}

#define GRID_SIZE 0x28
void liberty_callback_draw(double deltatime)
{
	liberty_set_draw_layer(uiLayer);
	liberty_set_colour((LibertyRGB){0x00, 0x00, 0x00, 0xff});
	liberty_draw_clear();

	liberty_set_colour((LibertyRGB){0x20, 0x20, 0x20, 0xff});
	for (uint8_t x = 0; x <= WIDTH/GRID_SIZE; x++)
		for (uint8_t y = 0; y <= HEIGHT/GRID_SIZE; y++)
		{
			liberty_draw_line((LibertyVec2){x*GRID_SIZE-(int)p.camera.x%GRID_SIZE - 1, y*GRID_SIZE-(int)p.camera.y%GRID_SIZE}, (LibertyVec2){x*GRID_SIZE-(int)p.camera.x%GRID_SIZE + 1, y*GRID_SIZE-(int)p.camera.y%GRID_SIZE});
			liberty_draw_line((LibertyVec2){x*GRID_SIZE-(int)p.camera.x%GRID_SIZE, y*GRID_SIZE-(int)p.camera.y%GRID_SIZE - 1}, (LibertyVec2){x*GRID_SIZE-(int)p.camera.x%GRID_SIZE, y*GRID_SIZE-(int)p.camera.y%GRID_SIZE + 1});
		}

	liberty_set_colour((LibertyRGB){0xff, 0x50, 0x50, 0xff});
	liberty_draw_rect(0, (LibertyRect){20, 20, 0x10, 0x10});

	liberty_set_colour((LibertyRGB){0x50, 0xff, 0x50, 0xff});
	liberty_draw_rect(0, (LibertyRect){30, 30, 0x10, 0x10});

	liberty_set_colour((LibertyRGB){0x50, 0x50, 0xff, 0xff});
	liberty_draw_rect(0, (LibertyRect){40, 40, 0x10, 0x10});

	liberty_draw_entities(p.camera);

	liberty_set_colour((LibertyRGB){0xff, 0xff, 0xff, 0xff});

	liberty_draw_rect(0, (LibertyRect){p.pos.x - p.camera.x, p.pos.y - p.camera.y, PLAYER_SIZE, PLAYER_SIZE});

	draw_dialogue(dialogue, deltatime);
	char buffer[32];
	snprintf(buffer, 32, "%d", (int)(1.0 / deltatime));
	liberty_draw_font_string(font, (LibertyVec2){WIDTH - 40, 1}, buffer);
	if (p.input.up) liberty_draw_font_string(font, (LibertyVec2){WIDTH - 40, 10}, "up");
	if (p.input.down) liberty_draw_font_string(font, (LibertyVec2){WIDTH - 40, 15}, "down");
	if (p.input.left) liberty_draw_font_string(font, (LibertyVec2){WIDTH - 40, 20}, "left");
	if (p.input.right) liberty_draw_font_string(font, (LibertyVec2){WIDTH - 40, 25}, "right");

	snprintf(buffer, 32, "pos: %d x %d", (int)p.pos.x, (int)p.pos.y);
	liberty_draw_font_string(font, (LibertyVec2){WIDTH - 80, 45}, buffer);

	snprintf(buffer, 32, "camera: %d x %d", (int)p.camera.x, (int)p.camera.y);
	liberty_draw_font_string(font, (LibertyVec2){WIDTH - 80, 50}, buffer);

	liberty_set_draw_layer(NULL);

	int rng;
	for (uint16_t i = 0; i < HEIGHT; i++)
	{
		rng = rand()%6;

		/* shadow */
		liberty_draw_layer_row(uiLayer, i, SDL_BLENDMODE_ADD, (LibertyVec2){6 + rng +1, -2 + -1}, (LibertyRGB){0xff, 0x00, 0x00, 0x30});
		liberty_draw_layer_row(uiLayer, i, SDL_BLENDMODE_ADD, (LibertyVec2){6 + rng -1, -2 + +0}, (LibertyRGB){0x00, 0xff, 0x00, 0x30});
		liberty_draw_layer_row(uiLayer, i, SDL_BLENDMODE_ADD, (LibertyVec2){6 + rng +2, -2 + +1}, (LibertyRGB){0x00, 0x00, 0xff, 0x30});

		/* TODO: bloom */
		liberty_draw_layer_row(uiLayer, i, SDL_BLENDMODE_ADD, (LibertyVec2){rng +1, -1}, (LibertyRGB){0xff, 0x00, 0x00, 0xff});
		liberty_draw_layer_row(uiLayer, i, SDL_BLENDMODE_ADD, (LibertyVec2){rng -1, +0}, (LibertyRGB){0x00, 0xff, 0x00, 0xff});
		liberty_draw_layer_row(uiLayer, i, SDL_BLENDMODE_ADD, (LibertyVec2){rng +2, +1}, (LibertyRGB){0x00, 0x00, 0xff, 0xff});
		liberty_draw_layer_row(uiLayer, i, SDL_BLENDMODE_ADD, (LibertyVec2){0,       0}, (LibertyRGB){0xff, 0xff, 0xff, 0xff});
	}

	// liberty_draw_layer(noiseLayer, SDL_BLENDMODE_ADD, (LibertyVec2){rand()%(NOISE_TEX_SIZE - WIDTH), rand()%(NOISE_TEX_SIZE - HEIGHT)}, (LibertyRGB){0x50, 0x40, 0xff, 0x20});
}

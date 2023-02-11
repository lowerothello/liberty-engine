#include "../include/liberty.h"

#define WIDTH 320
#define HEIGHT 180
#define NOISE_TEX_SIZE 2048

LibertyConfig liberty_default_config =
{
	.title = "Liberty",
	.width = WIDTH,
	.height = HEIGHT,
	.scale = 3,
	.fullscreen = 0,
	.vsync = 0,
	.framerate = 60.0f,
};

LibertyFont  *font;
LibertyLayer *uiLayer, *noiseLayer;

void liberty_callback_init(void)
{
	font = liberty_new_font_from_file("Untitled1-6.bdf");
	uiLayer = liberty_new_layer((LibertyVec2){WIDTH, HEIGHT});
	noiseLayer = liberty_new_layer((LibertyVec2){NOISE_TEX_SIZE, NOISE_TEX_SIZE});
	liberty_set_draw_layer(noiseLayer);
	liberty_set_colour((LibertyColour){0x00, 0x00, 0x00, 0xff});
	liberty_draw_clear();

	for (uint16_t x = 0; x < NOISE_TEX_SIZE; x++)
		for (uint16_t y = 0; y < NOISE_TEX_SIZE; y++)
		{
			liberty_set_colour((LibertyColour){0xff, 0xff, 0xff, rand()%256});
			liberty_draw_pixel((LibertyVec2){x, y});
		}
	liberty_set_draw_layer(NULL);
}

void liberty_callback_cleanup(void)
{
	liberty_free_font(font);
	liberty_free_layer(uiLayer);
	liberty_free_layer(noiseLayer);
}


typedef struct Player
{
	LibertyVec2 pos;
	LibertyVec2 accel;

	struct {
		bool up;
		bool down;
		bool left;
		bool right;
	} input;
} Player;
Player p;

#define WALK_ACCEL 2000.f
#define WALK_FRICTION 600.f
#define WALK_TERMINAL 100.f

LibertySignal liberty_callback_event(SDL_Event event)
{
	switch (event.type)
	{
		case SDL_EVENT_KEY_DOWN: /* fall through */
		case SDL_EVENT_KEY_UP:
			if (event.key.repeat) break;
			switch (event.key.keysym.sym)
			{
				case SDLK_UP: p.input.up = (event.type == SDL_EVENT_KEY_UP) ? 0 : 1; break;
				case SDLK_DOWN: p.input.down = (event.type == SDL_EVENT_KEY_UP) ? 0 : 1; break;
				case SDLK_LEFT: p.input.left = (event.type == SDL_EVENT_KEY_UP) ? 0 : 1; break;
				case SDLK_RIGHT: p.input.right = (event.type == SDL_EVENT_KEY_UP) ? 0 : 1; break;
			}
			break;
	}
	return LIBERTY_SIGNAL_OK;
}

void handle_accel(double deltatime, LibertyVec2 *accel,
		bool up, bool down, bool left, bool right,
		float coef_speed, float coef_friction, float coef_max)
{
	if (up)    accel->y -= coef_speed*deltatime;
	if (down)  accel->y += coef_speed*deltatime;
	if (left)  accel->x -= coef_speed*deltatime;
	if (right) accel->x += coef_speed*deltatime;

	if (accel->x > 0.0f) accel->x = MIN(accel->x - coef_friction*deltatime,  coef_max);
	if (accel->x < 0.0f) accel->x = MAX(accel->x + coef_friction*deltatime, -coef_max);

	if (accel->y > 0.0f) accel->y = MIN(accel->y - coef_friction*deltatime,  coef_max);
	if (accel->y < 0.0f) accel->y = MAX(accel->y + coef_friction*deltatime, -coef_max);
}

LibertySignal liberty_callback_update(double deltatime)
{

	handle_accel(deltatime, &p.accel,
			p.input.up, p.input.down, p.input.left, p.input.right,
			WALK_ACCEL, WALK_FRICTION, WALK_TERMINAL);

	p.pos.x += p.accel.x*deltatime;
	p.pos.y += p.accel.y*deltatime;

	return LIBERTY_SIGNAL_OK;
}

void liberty_callback_draw(double frametime)
{
	liberty_set_draw_layer(uiLayer);
	liberty_set_colour((LibertyColour){0x00, 0x00, 0x00, 0xff});
	liberty_draw_clear();

	liberty_set_colour((LibertyColour){0xff, 0x50, 0x50, 0xff});
	liberty_draw_rect(0, (LibertyVec4){20, 20, 0x10, 0x10});

	liberty_set_colour((LibertyColour){0x50, 0xff, 0x50, 0xff});
	liberty_draw_rect(0, (LibertyVec4){30, 30, 0x10, 0x10});

	liberty_set_colour((LibertyColour){0x50, 0x50, 0xff, 0xff});
	liberty_draw_rect(0, (LibertyVec4){40, 40, 0x10, 0x10});

	liberty_set_colour((LibertyColour){0xff, 0xff, 0xff, 0xff});
	LIBERTY_DRAW_FONT_STRING_CENTRE(font, ((LibertyVec4){0, HEIGHT>>1, WIDTH, HEIGHT>>1}), "i'm lost on what to do");
	liberty_draw_rect(0, (LibertyVec4){(WIDTH>>1) + p.pos.x, (HEIGHT>>1) + p.pos.y, 0x10, 0x10});

	liberty_set_draw_layer(NULL);

	int rng;
	for (uint16_t i = 0; i < HEIGHT; i++)
	{
		rng = rand()%3;
		liberty_draw_layer_row(uiLayer, i, (LibertyVec2){rng +1, -1}, (LibertyColour){0xff, 0x00, 0x00, 0xff});
		liberty_draw_layer_row(uiLayer, i, (LibertyVec2){rng -1, +0}, (LibertyColour){0x00, 0xff, 0x00, 0xff});
		liberty_draw_layer_row(uiLayer, i, (LibertyVec2){rng +2, +1}, (LibertyColour){0x00, 0x00, 0xff, 0xff});
	}

	liberty_draw_layer(noiseLayer, (LibertyVec2){rand()%(NOISE_TEX_SIZE - WIDTH), rand()%NOISE_TEX_SIZE - HEIGHT}, (LibertyColour){0x00, 0x00, 0xff, 0x20});

	char buffer[32];
	snprintf(buffer, 32, "%d", (int)(1.0 / frametime));
	liberty_draw_font_string(font, (LibertyVec2){WIDTH - 40, 1}, buffer);
	if (p.input.up) liberty_draw_font_string(font, (LibertyVec2){WIDTH - 40, 10}, "up");
	if (p.input.down) liberty_draw_font_string(font, (LibertyVec2){WIDTH - 40, 15}, "down");
	if (p.input.left) liberty_draw_font_string(font, (LibertyVec2){WIDTH - 40, 20}, "left");
	if (p.input.right) liberty_draw_font_string(font, (LibertyVec2){WIDTH - 40, 25}, "right");
	snprintf(buffer, 32, "%d x %d", (int)p.pos.x, (int)p.pos.y);
	liberty_draw_font_string(font, (LibertyVec2){WIDTH - 80, 45}, buffer);
}

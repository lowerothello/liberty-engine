typedef struct LibertyRGB
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
} LibertyRGB;

typedef struct LibertyHSV
{
	uint8_t h;
	uint8_t s;
	uint8_t v;
	uint8_t a;
} LibertyHSV;

LibertyHSV liberty_rgb_to_hsv(LibertyRGB);
LibertyRGB liberty_hsv_to_rgb(LibertyHSV);

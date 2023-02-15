/* https://stackoverflow.com/a/6930407 */
LibertyHSV liberty_rgb_to_hsv(LibertyRGB in)
{
	LibertyHSV out;
	out.a = in.a;

	uint8_t min = MIN(MIN(in.r, in.g), in.b);
	uint8_t max = MAX(MAX(in.r, in.g), in.b);
	float  fmax = MAX(MAX(in.r, in.g), in.b)*255.0f;

	out.v = max;
	uint8_t delta = max - min;
	float fdelta = fmax - min*255.0f;

	if (!delta)
	{
		out.s = 0;
		out.h = 0;
		return out;
	}

	if (max)
		out.s = delta / max;
	else
	{
		out.s = 0;
		out.h = 0;
		return out;
	}

	if      (in.r >= max) out.h = (0.0f + ((in.g - in.b)*255.0f) / fdelta) * 42.5f; /* 42.5 == 255 / 6 */
	else if (in.g >= max) out.h = (2.0f + ((in.b - in.r)*255.0f) / fdelta) * 42.5f;
	else                  out.h = (4.0f + ((in.r - in.g)*255.0f) / fdelta) * 42.5f;

	return out;
}

LibertyRGB liberty_hsv_to_rgb(LibertyHSV in)
{
	LibertyRGB out;
	out.a = in.a;

	if (!in.s)
	{
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}

	float hh = in.h / 42.5f; /* 42.5 == 255 / 6 */
	int i = hh; /* chopped off after radix point */
	float ff = hh - i;
	float fv = in.v*DIV255;
	float p = fv * (1.0f -  in.s*DIV255);
	float q = fv * (1.0f - (in.s*DIV255 * ff));
	float t = fv * (1.0f - (in.s*DIV255 * (1.0f - ff)));

	switch (i)
	{
		case 0:  out.r = in.v; out.g = t*255.0f; out.b = p*255.0f; break;
		case 1:  out.r = q*255.0f; out.g = in.v; out.b = p*255.0f; break;
		case 2:  out.r = p*255.0f; out.g = in.v; out.b = t*255.0f; break;
		case 3:  out.r = p*255.0f; out.g = q*255.0f; out.b = in.v; break;
		case 4:  out.r = t*255.0f; out.g = p*255.0f; out.b = in.v; break;
		default: out.r = in.v; out.g = p*255.0f; out.b = q*255.0f; break;
	}

	return out;
}

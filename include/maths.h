#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define CLAMP(x, y, z) ((x) < (y) ? ((x) > (z) ? (x) : (z)) : (y))
#define LERP(x, y, z) ((x) + ((y) - (x)) * (z))

#define DIV255 0.00392156862745098f /* 1/255 */

typedef struct LibertyVec2
{
	float x;
	float y;
} LibertyVec2;

typedef struct LibertyVec3
{
	float x;
	float y;
	float z;
} LibertyVec3;

typedef struct LibertyVec4
{
	float x;
	float y;
	float w;
	float h;
} LibertyVec4;

typedef struct LibertyIntVec4
{
	int x;
	int y;
	int w;
	int h;
} LibertyIntVec4;

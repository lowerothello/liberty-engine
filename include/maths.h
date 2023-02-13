/* returns whichever is lesser between x and y */
#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* returns wichever is greater between x and y */
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* returns x, ensured to be above y and below z */
#define CLAMP(x, y, z) ((x) < (y) ? ((x) > (z) ? (x) : (z)) : (y))

/* returns a point (float)z along the line between x and y */
#define LERP(x, y, z) ((x) + ((y) - (x)) * (z))


#define DIV255 0.00392156862745098f /* 1/255 */


/* point in 2d space */
typedef struct LibertyVec2
{
	float x;
	float y;
} LibertyVec2;

/* point in 3d space */
typedef struct LibertyVec3
{
	float x;
	float y;
	float z;
} LibertyVec3;

/* float 2d rectangle */
typedef struct LibertyRect
{
	float x;
	float y;
	float w;
	float h;
} LibertyRect;

/* int 2d rectangle */
typedef struct LibertyIntRect
{
	int x;
	int y;
	int w;
	int h;
} LibertyIntRect;

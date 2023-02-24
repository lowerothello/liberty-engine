typedef struct LibertyEntityDB
{
	unsigned short c;
	LibertyEntity  v[USHRT_MAX+1]; /* 512kb */
} LibertyEntityDB;
LibertyEntityDB edb;

static void free_entity_state(void)
{
	for (unsigned short i = 0; i < edb.c; i++)
		liberty_free_entity(&edb.v[i]);
	edb.c = 0;
}

static unsigned short get_empty_entity(void)
{
	/* return the first invalid entity */
	for (unsigned short i = 0; i < edb.c; i++)
		if (!edb.v[i].type)
			return i;

	/* no entities are invalid */
	edb.c++;
	return edb.c - 1;
}

/* .data is the starting state, it can be omitted */
LibertyEntity *liberty_add_entity(uint8_t type, LibertyRect collision,
		void *data, size_t datasize,
		void (*update)(struct LibertyEntity *, float deltatime),
		void   (*draw)(struct LibertyEntity *, LibertyVec2 camera))
{
	unsigned short index = get_empty_entity();
	edb.v[index].type = type;
	edb.v[index].collision = collision;
	edb.v[index].data = malloc(datasize);

	/* initialize data if a starting state has been passed */
	if (data)
		memcpy(edb.v[index].data, data, datasize);

	edb.v[index].draw = draw;
	edb.v[index].update = update;

	return &edb.v[index];
}

void liberty_free_entity(LibertyEntity *entity)
{
	if (entity->data) free(entity->data);
	entity->type = 0;
}

void liberty_update_entities(float deltatime)
{
	for (unsigned short i = 0; i < edb.c; i++)
		if (edb.v[i].type && edb.v[i].update)
			edb.v[i].update(&edb.v[i], deltatime);
}
void liberty_draw_entities(LibertyVec2 camera)
{
	for (unsigned short i = 0; i < edb.c; i++)
		if (edb.v[i].type && edb.v[i].draw)
			edb.v[i].draw(&edb.v[i], camera);
}

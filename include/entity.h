typedef struct LibertyEntity
{
	uint8_t        type;      /* 0 if the entity is invalid */
	LibertyRect    collision; /* collision rectangle */
	void (*update)(struct LibertyEntity *, float deltatime); /* update the entity's state */
	void   (*draw)(struct LibertyEntity *, LibertyVec2 camera); /* draw the entity */
	void          *data;      /* arbitrary entity data */
} LibertyEntity;

void liberty_add_entity(uint8_t type, LibertyRect collision,
		void *data, size_t datasize,
		void (*update)(struct LibertyEntity *, float deltatime),
		void   (*draw)(struct LibertyEntity *, LibertyVec2 camera));

void liberty_free_entity(LibertyEntity *entity);

void liberty_update_entities(float deltatime);
void liberty_draw_entities(LibertyVec2 camera);

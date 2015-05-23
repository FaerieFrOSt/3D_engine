
#ifndef _SECTOR_H_
# define _SECTOR_H_

#include "vertice.h"
#include "sdl_init.h"
#include "player.h"

struct sector {
	int	numVertices;
	float	floor, ceiling;
	struct vertice	**vertex;
};

struct sector	*create_sector(float floor, float ceiling);
void			delete_sector(struct sector**);
void			addVertice(struct sector*, float x, float y);

void			drawSector(SDL_Surface*, struct player*, struct sector*, uint8_t);

#endif /* !_SECTOR_H_ */

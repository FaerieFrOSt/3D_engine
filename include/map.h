
#ifndef _MAP_H_
# define _MAP_H_

#include "sector.h"
#include "sdl_init.h"
#include "player.h"

struct map {
	struct sector	**sectors;
	int				nbSectors;
	struct {
		int	x, y, sector;
		double	angle;
	} beginPosition;
};

struct map	*load_map(const struct sdl_data *, const char *filename);
void		delete_map(struct map**);

void		drawMap(SDL_Surface*, struct player*, struct map*);

#endif /* !_MAP_H_ */


#ifndef _SECTOR_H_
# define _SECTOR_H_

#include "vertice.h"

struct sector {
	int	numVertices;
	int	floor, ceiling;
	struct vertice	**vertex;
};

struct sector	*create_sector(int floor, int ceiling);
void			delete_sector(struct sector**);
void			addVertice(struct sector*, int x, int y);

#endif /* !_SECTOR_H_ */


#ifndef _PLAYER_H_
# define _PLAYER_H_

#include "sdl_init.h"

#define PLAYER_SPEED	10

struct player {
	int		x, y, z;
	float	angle, anglecos, anglesin;
	int		speed;
	struct {
		int	up;
		int	down;
		int	left;
		int	right;
	} keys;
};

struct player	*init_player(int x, int y, int z, float angle);
void			delete_player(struct player **);

void			updatePlayerPosition(struct sdl_data*, struct player*, int dx, int dy);
void			updatePlayerRotation(struct player*, float rot);

#endif /* !_PLAYER_H_ */

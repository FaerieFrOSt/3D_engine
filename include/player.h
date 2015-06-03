
#ifndef _PLAYER_H_
# define _PLAYER_H_

#include "sdl_init.h"

#define PLAYER_SPEED	2
#define EYEHEIGHT		6
#define MAXYAW			1.5f

struct player {
	int		x, y, z;
	float	angle, anglecos, anglesin;
	float	yaw;
	int		speed;
	float	sensibility;
	float	ysensibility;
	int		sector;
	struct {
		int	forward;
		int	backward;
		int	left;
		int	right;
		int	rotateLeft;
		int	rotateRight;
	} keys;
};

struct player	*init_player(int x, int y, int z, float angle);
void			delete_player(struct player **);

void			updatePlayerPosition(struct sdl_data*, struct player*, int dx, int dy);
void			updatePlayerRotation(struct player*, float rot, float yaw);

#endif /* !_PLAYER_H_ */

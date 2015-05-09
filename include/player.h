
#ifndef _PLAYER_H_
# define _PLAYER_H_

struct player {
	int		x, y, z;
	float	angle, anglecos, anglesin;
	struct {
		int	up;
		int	down;
		int	left;
		int	right;
	} keys;
};

struct player	*init_player(int x, int y, int z, float angle);
void			delete_player(struct player **);

#endif /* !_PLAYER_H_ */

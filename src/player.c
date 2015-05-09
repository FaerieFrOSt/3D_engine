#include "player.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

struct player	*init_player(int x, int y, int z, float angle) {
	struct player	*tmp = malloc(sizeof(*tmp));
	tmp->x = x;
	tmp->y = y;
	tmp->z = z;
	tmp->angle = angle;
	tmp->anglecos = cosf(angle);
	tmp->anglesin = sinf(angle);
	tmp->keys.up = SDL_SCANCODE_W;
	tmp->keys.down = SDL_SCANCODE_S;
	tmp->keys.left = SDL_SCANCODE_A;
	tmp->keys.right = SDL_SCANCODE_D;
	return tmp;
}

void	delete_player(struct player **player) {
	if (!player || !*player)
		return;
	free(*player);
	*player = NULL;
}

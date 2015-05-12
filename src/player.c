#include "player.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <math.h>

struct player	*init_player(int x, int y, int z, float angle) {
	struct player	*tmp = malloc(sizeof(*tmp));
	tmp->x = x;
	tmp->y = y;
	tmp->z = z;
	tmp->speed = PLAYER_SPEED;
	tmp->angle = angle * M_PI / 180.f;
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

void	updatePlayerPosition(struct sdl_data *data, struct player *player, int dx, int dy) {
	int	x = dx * player->speed,
		y = dy * player->speed;

	x = x * player->anglecos - y * player->anglesin;
	y = x * player->anglesin + y * player->anglecos;

	player->x += x;
	player->y += y;
}

void	updatePlayerRotation(struct player *player, float rot) {
	player->angle += rot * M_PI / 180.f;
	while (player->angle < 0.f)
		player->angle += 2.f * M_PI;
	while (player->angle > 2.f * M_PI)
		player->angle -= 2.f * M_PI;
	printf("%.2f\n", player->angle);
	player->anglecos = cosf(player->angle);
	player->anglesin = sinf(player->angle);
}

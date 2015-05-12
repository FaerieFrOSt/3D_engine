#include <stdio.h>
#include "sdl_init.h"
#include "draw.h"
#include "player.h"
#include "utils.h"

int	main(int argc, char *argv[]) {
	struct sdl_data	*data = init(argv[0], "3D engine");
	struct player	*player = init_player(W / 2, H / 2, 0, 0);
	data->fps.print = 1;
	data->fps.capFPS = 1;
	while (!keyPressed(data, SDL_SCANCODE_ESCAPE) && !data->events.quit) {
		updateEvents(data);
		int	dx = 0, dy = 0;
		if (keyPressed(data, player->keys.up))
			dy = -1;
		else if (keyPressed(data, player->keys.down))
			dy = 1;
		if (keyPressed(data, player->keys.left))
			dx = -1;
		else if (keyPressed(data, player->keys.right))
			dx = 1;
		updatePlayerPosition(data, player, dx, dy);
		int	x;
		SDL_GetRelativeMouseState(&x, NULL);
		updatePlayerRotation(player, x);

		SDL_FillRect(data->screen, NULL, SDL_MapRGB(data->screen->format, 0, 0, 0));
		// wall
		drawVLine(data->screen, W / 2 + 50, H / 2 - 25, H / 2 + 25, SDL_MapRGB(data->screen->format, 255, 0, 0));
		// player's position
		drawVLine(data->screen, player->x, player->y - 2, player->y + 2, SDL_MapRGB(data->screen->format, 0, 255, 0));
		drawHLine(data->screen, player->x - 2, player->x + 2, player->y, SDL_MapRGB(data->screen->format, 0, 255, 0));
		display(data);
	}
	delete_player(&player);
	end(&data);
	return 0;
}

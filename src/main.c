#include <stdio.h>
#include "sdl_init.h"
#include "draw.h"
#include "player.h"

int	main(int argc, char *argv[]) {
	struct sdl_data	*data = init(argv[0], "3D engine");
	struct player	*player = init_player(10, 10, 0, 0);
	data->fps.print = 1;
	data->fps.capFPS = 1;
	while (!keyPressed(data, SDL_SCANCODE_ESCAPE) && !data->events.quit) {
		updateEvents(data);
		if (keyPressed(data, player->keys.up))
			player->y -= 1;
		else if (keyPressed(data, player->keys.down))
			player->y += 1;
		if (keyPressed(data, player->keys.left))
			player->x -= 1;
		else if (keyPressed(data, player->keys.right))
			player->x += 1;

		SDL_FillRect(data->screen, NULL, SDL_MapRGB(data->screen->format, 0, 0, 0));
		// wall
		drawVLine(data->screen, 50, 50, 10, SDL_MapRGB(data->screen->format, 255, 0, 0));
		// player's position
		drawVLine(data->screen, player->x, player->y - 2, player->y + 2, SDL_MapRGB(data->screen->format, 0, 255, 0));
		drawHLine(data->screen, player->x - 2, player->x + 2, player->y, SDL_MapRGB(data->screen->format, 0, 255, 0));
		display(data);
	}
	delete_player(&player);
	end(&data);
	return 0;
}

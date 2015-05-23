#include <stdio.h>
#include "sdl_init.h"
#include "draw.h"
#include "player.h"
#include "utils.h"
#include "map.h"

int	main(int argc, char *argv[]) {
	struct sdl_data	*data = init(argv[0], "3D engine");
	data->fps.print = 1;
	data->fps.capFPS = 1;
	struct map	*map = load_map(data, "map.txt");
	struct player	*player = init_player(map->beginPosition.x, map->beginPosition.y, map->sectors[map->beginPosition.sector]->floor, map->beginPosition.angle);
	data->player = player;
	while (!keyPressed(data, SDL_SCANCODE_ESCAPE) && !data->events.quit) {
		updateEvents(data);
		int	dx = 0, dy = 0;
		if (keyPressed(data, player->keys.forward))
			dx = 1;
		else if (keyPressed(data, player->keys.backward))
			dx = -1;
		if (keyPressed(data, player->keys.left))
			dy = -1;
		else if (keyPressed(data, player->keys.right))
			dy = 1;
		if (keyPressed(data, player->keys.rotateLeft))
			updatePlayerRotation(player, 20*player->sensibility);
		else if (keyPressed(data, player->keys.rotateRight))
			updatePlayerRotation(player, -20*player->sensibility);
		updatePlayerPosition(data, player, dx, dy);
		int	x;
		SDL_GetRelativeMouseState(&x, NULL);
		updatePlayerRotation(player, x * player->sensibility);

		SDL_FillRect(data->screen, NULL, SDL_MapRGB(data->screen->format, 0, 0, 0));
		// wall
		drawMap(data, map);
		// player's position
		drawVLine(data->screen, W/2, H/2 - 2, H/2+2, SDL_MapRGB(data->screen->format, 0, 255, 0));
		drawHLine(data->screen, W/2-2, W/2+2, H/2, SDL_MapRGB(data->screen->format, 0, 255, 0));
		/* drawVLine(data->screen, player->x, player->y - 2, player->y + 2, SDL_MapRGB(data->screen->format, 0, 255, 0)); */
		/* drawHLine(data->screen, player->x - 2, player->x + 2, player->y, SDL_MapRGB(data->screen->format, 0, 255, 0)); */
		// player's direction view
		drawLine(data->screen, W/2, H/2, W/2, H/2 - 20, SDL_MapRGB(data->screen->format, 0, 0, 255));
		/* drawLine(data->screen, player->x, player->y, */
		/* 		player->anglecos * 20 + player->x, player->anglesin * 20 + player->y, */
				/* SDL_MapRGB(data->screen->format, 0, 0, 255)); */
		// draw everything on screen
		display(data);
	}
	delete_player(&player);
	end(&data);
	return 0;
}

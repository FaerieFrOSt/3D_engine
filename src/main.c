#include <stdio.h>
#include "sdl_init.h"
#include "draw.h"
#include "player.h"
#include "utils.h"
#include "map.h"
#include "3D.h"

int	main(int argc, char *argv[]) {
	struct sdl_data	*data = init(argv[0], "3D engine");
	data->fps.print = 1;
	data->fps.capFPS = 1;
	struct map	*map = load_map(data, "map.txt");
	struct player	*player = init_player(map->beginPosition.x, map->beginPosition.y, map->sectors[map->beginPosition.sector - 1]->floor, map->beginPosition.angle);
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
		SDL_GetRelativeMouseState(&dx, &dy);
		updatePlayerRotation(player, dx * player->sensibility);

		SDL_FillRect(data->screen, NULL, SDL_MapRGB(data->screen->format, 0, 0, 0));
		SDL_FillRect(data->minimap, NULL, SDL_MapRGB(data->minimap->format, 0, 0, 0));
		drawMinimap(data, map);
		display(data);
	}
	delete_player(&player);
	end(&data);
	return 0;
}

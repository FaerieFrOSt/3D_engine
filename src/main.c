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
		// wall
		drawVLine(data->screen, 50, 50, 10, SDL_MapRGB(data->screen->format, 255, 0, 0));
		// player's position
		drawVLine(data->screen, player->x, player->y - 1, player->y + 1, SDL_MapRGB(data->screen->format, 0, 255, 0));
		display(data);
	}
	end(&data);
	return 0;
}

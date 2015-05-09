#include <stdio.h>
#include "sdl_init.h"
#include "draw.h"

int	main(int argc, char *argv[]) {
	struct sdl_data	*data = init(argv[0], "3D engine");
	data->fps.print = 1;
	data->fps.capFPS = 1;
	drawVLine(data->screen, 50, 50, 10, SDL_MapRGB(data->screen->format, 255, 0, 0));
	while (!data->events.key[SDLK_ESCAPE] && !data->events.quit) {
		updateEvents(data);
		display(data);
	}
	end(&data);
	return 0;
}

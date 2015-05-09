#include <stdio.h>
#include "sdl.h"

int	main() {
	struct sdl_data	*data = init("3D engine");
	data->fps.print = 1;
	data->fps.capFPS = 1;
	while (!data->events.key[SDLK_ESCAPE] && !data->events.quit) {
		updateEvents(data);
		print(data);
	}
	end(&data);
	return 0;
}

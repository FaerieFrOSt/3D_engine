#include "3D.h"
#include "draw.h"

void	drawPerspective(struct sdl_data *data, struct map *map) {
}

void	drawMinimap(struct sdl_data *data, struct map *map) {
	// walls
	drawMap(data->minimap, data->player, map);
	// player position
	drawVLine(data->minimap, data->minimap->w / 2, data->minimap->h / 2 - 2,
			data->minimap->h / 2 + 2, SDL_MapRGB(data->minimap->format, 0, 255, 0));
	drawHLine(data->minimap, data->minimap->w / 2 - 2, data->minimap->w / 2 + 2,
			data->minimap->h / 2, SDL_MapRGB(data->minimap->format, 0, 255, 0));
	// player direction
	drawVLine(data->minimap, data->minimap->w / 2, data->minimap->h / 2,
			data->minimap->h/2 - 10, SDL_MapRGB(data->minimap->format, 0, 0, 255));
}

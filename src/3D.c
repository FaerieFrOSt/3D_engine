#include "3D.h"
#include "draw.h"

void	drawPerspective(struct sdl_data *data, struct map *map) {
}

static inline void	fill(SDL_Surface *s, unsigned int x, unsigned int y, const uint32_t color) {
	if (x < 0 || x >= s->w || y < 0 || y >= s->h)
		return;
	if (((uint32_t*)s->pixels)[y * s->w + x])
		return;
	drawPixel(s, x, y, color);
	fill(s, x - 1, y, color);
	fill(s, x + 1, y, color);
	fill(s, x, y - 1, color);
	fill(s, x, y + 1, color);
}

void	drawMinimap(struct sdl_data *data, struct map *map) {
	// walls
	drawMap(data->minimap, data->player, map, data->debug);
	//fill algorithm
	if (((struct player*)data->player)->sector != -1)
		fill(data->minimap, data->minimap->w / 2, data->minimap->h / 2, SDL_MapRGB(data->minimap->format, 0, 0, 50));
	// player position
	drawVLine(data->minimap, data->minimap->w / 2, data->minimap->h / 2 - 2,
			data->minimap->h / 2 + 2, SDL_MapRGB(data->minimap->format, 0, 255, 0));
	drawHLine(data->minimap, data->minimap->w / 2 - 2, data->minimap->w / 2 + 2,
			data->minimap->h / 2, SDL_MapRGB(data->minimap->format, 0, 255, 0));
	// player direction
	drawVLine(data->minimap, data->minimap->w / 2, data->minimap->h / 2,
			data->minimap->h/2 - 10, SDL_MapRGB(data->minimap->format, 255, 0, 255));
}

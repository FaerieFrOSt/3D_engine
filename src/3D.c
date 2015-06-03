#include "3D.h"
#include "draw.h"
#include "utils.h"

void	drawPerspective(struct sdl_data *data, struct map *map) {
	struct player	*player = data->player;
	if (player->sector == -1)
		return;
	// render sector of the player
	int	i;
	const struct sector	* const s = map->sectors[player->sector];
	for (i = 0; i < s->numVertices - 1; ++i) {
		float	vx1 = s->vertex[i]->x - player->x;
		float	vy1 = s->vertex[i]->y - player->y;
		float	vx2 = s->vertex[i + 1]->x - player->x;
		float	vy2 = s->vertex[i + 1]->y - player->y;
		float	tx1 = vx1 * player->anglesin - vy1 * player->anglecos;
		float	tz1 = vx1 * player->anglecos + vy1 * player->anglesin;
		float	tx2 = vx2 * player->anglesin - vy2 * player->anglecos;
		float	tz2 = vx2 * player->anglecos + vy2 * player->anglesin;
		if (tz1 <= 0 && tz2 <= 0)
			continue; // outside player view
		if (tz1 <= 0 || tz2 <= 0) { // partially outside player view
			// find an intersection between wall and approximate edges of player's view
			struct vertice	i1, i2;
			Intersect(tx1, tz1, tx2, tz2, -NEARSIDE, NEARZ, -FARSIDE, FARZ, i1.x, i1.y);
			Intersect(tx1, tz1, tx2, tz2, NEARSIDE, NEARZ, FARSIDE, FARZ, i2.x, i2.y);
			if (tz1 < NEARZ) {
				if (i1.y > 0) {
					tx1 = i1.x;
					tz1 = i1.y;
				} else {
					tx1 = i2.x;
					tz1 = i2.y;
				}
			}
			if (tz2 < NEARZ) {
				if (i1.y > 0) {
					tx2 = i1.x;
					tz2 = i1.y;
				} else {
					tx2 = i2.x;
					tz2 = i2.y;
				}
			}
		}
		// perspective transformations
		float	xscale1 = (float)HFOV / tz1;
		float	yscale1 = (float)VFOV / tz1;
		int		x1 = data->screen->w / 2.f - tx1 * xscale1;
		float	xscale2 = (float)HFOV / tz2;
		float	yscale2 = (float)VFOV / tz2;
		int		x2 = data->screen->w / 2.f - tx2 * xscale2;
		if (x1 >= x2)
			continue; // only render if visible
		float	yceil = s->ceiling - player->z;
		float	yfloor = s->floor - player->z;
		#define Yaw(y, z) (y + z * player->yaw)
		int	y1a = data->screen->h / 2.f - Yaw(yceil, tz1) * yscale1;
		int	y1b = data->screen->h / 2.f - Yaw(yfloor, tz1) * yscale1;
		int	y2a = data->screen->h / 2.f - Yaw(yceil, tz2) * yscale2;
		int	y2b = data->screen->h / 2.f - Yaw(yfloor, tz2) * yscale2;
		// render wall
		int	beginx = max(x1, 0), endx = min(x2, data->screen->w - 1);
		int	x;
		for (x = beginx; x <= endx; ++x) {
			// y coordinates for ceiling and floor for this x coordinate and clamp them
			int	ya = (x - x1) * (y2a - y1a) / (x2 - x1) + y1a;
			int	cya = clamp(ya, 0, data->screen->w - 1); // top
			int	yb = (x - x1) * (y2b - y1b) / (x2 - x1) + y1b;
			int	cyb = clamp(yb, 0, data->screen->w - 1); // bottom
			// render ceiling
			drawVLineColor(data->screen, x, 0, cya - 1, 0x111111, 0x222222, 0x111111);
			// render floor
			drawVLineColor(data->screen, x, cyb + 1, data->screen->w - 1, 0x0000FF, 0x0000AA, 0x0000FF);
			if (s->neighboors[i] != -1) { // TODO : neighboor
				drawVLineColor(data->screen, x, cya, cyb, 0, x == x1 || x == x2 ? 0 : 0xAA0000, 0);
			} else {
				drawVLineColor(data->screen, x, cya, cyb, 0, x == x1 || x == x2 ? 0 : 0xAAAAAA, 0);
			}
		}
	}
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

#include "3D.h"
#include "draw.h"
#include "utils.h"

void	drawPerspective(struct sdl_data *data, struct map *map) {
	struct player	*player = data->player;
	if (player->sector == -1)
		return;
	struct item	{ int sector, sx1, sx2; }	queue[MAXQUEUE], *head, *tail;
	int	*ytop, *ybottom, *renderedSectors;
	ytop = malloc(data->screen->w * sizeof(int));
	ybottom = malloc(data->screen->w * sizeof(int));
	renderedSectors = malloc(map->nbSectors * sizeof(int));
	memset(ytop, 0, data->screen->w * sizeof(int));
	memset(renderedSectors, 0, map->nbSectors * sizeof(int));
	int	i;
	for (i = 0; i < data->screen->w; ++i)
		ybottom[i] = data->screen->h - 1;
	head = queue;
	tail = queue;
	*head = (struct item) { player->sector, 0, data->screen->w - 1};
	if (++head == queue + MAXQUEUE)
		head = queue;
	// render sector of the player
	do {
		const struct item	now = *tail;
		if (++tail == queue + MAXQUEUE)
			tail = queue;
		if (renderedSectors[now.sector] & 0x21)
			continue;
		++renderedSectors[now.sector];
		const struct sector	* const s = map->sectors[now.sector];
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
			float	nyceil = 0, nyfloor = 0;
			if (s->neighboors[i] >= 0) {
				nyceil = map->sectors[(int)s->neighboors[i]]->ceiling;
				nyfloor = map->sectors[(int)s->neighboors[i]]->floor;
			}
			#define Yaw(y, z) (y + z * player->yaw)
			// project ceiling and floor into screen coordinates
			int	y1a = data->screen->h / 2 - (int)(Yaw(yceil, tz1) * yscale1);
			int	y1b = data->screen->h / 2 - (int)(Yaw(yfloor, tz1) * yscale1);
			int	y2a = data->screen->h / 2 - (int)(Yaw(yceil, tz2) * yscale2);
			int	y2b = data->screen->h / 2 - (int)(Yaw(yfloor, tz2) * yscale2);
			// same for neighboors
			int	ny1a = data->screen->h / 2 - (int)(Yaw(nyceil, tz1) * yscale1);
			int	ny1b = data->screen->h / 2 - (int)(Yaw(nyfloor, tz1) * yscale1);
			int	ny2a = data->screen->h / 2 - (int)(Yaw(nyceil, tz2) * yscale2);
			int	ny2b = data->screen->h / 2 - (int)(Yaw(nyfloor, tz2) * yscale2);
			// render wall
			int	beginx = max(x1, now.sx1), endx = min(x2, now.sx2);
			int	x;
			for (x = beginx; x <= endx; ++x) {
				// y coordinates for ceiling and floor for this x coordinate and clamp them
				int	ya = (x - x1) * (y2a - y1a) / (x2 - x1) + y1a;
				int	cya = clamp(ya, ytop[x], ybottom[x]); // top
				int	yb = (x - x1) * (y2b - y1b) / (x2 - x1) + y1b;
				int	cyb = clamp(yb, ytop[x], ybottom[x]); // bottom
				// render ceiling
				drawVLineColor(data->screen, x, ytop[x], cya - 1, 0x111111, 0x222222, 0x111111);
				// render floor
				drawVLineColor(data->screen, x, cyb + 1, ybottom[x], 0xF, 0xAA, 0xFF);
				if (s->neighboors[i] >= 0) { // neighboor
					int	nya = (x - x1) * (ny2a - ny1a) / (x2 - x1) + ny1a;
					int	cnya = clamp(nya, ytop[x], ybottom[x]);
					int	nyb = (x - x1) * (ny2b - ny1b) / (x2 - x1) + ny1b;
					int	cnyb = clamp(nyb, ytop[x], ybottom[x]);
					// if our ceiling is higher than their, render upper wall
					drawVLineColor(data->screen, x, cya, cnya - 1, 0, x == x1 || x == x2 ? 0 : 0x777777, 0);
					ytop[x] = clamp(max(cya, cnya), ytop[x], data->screen->h - 1); // shrink remaining window
					// draw bottom wal if our floor is lower than their floor
					drawVLineColor(data->screen, x, cnyb + 1, cyb, 0, x == x1 || x == x2 ? 0 : 0xCCCCCC, 0);
					ybottom[x] = clamp(min(cyb, cnyb), 0, ybottom[x]);
				} else {
					drawVLineColor(data->screen, x, cya, cyb, 0, x == x1 || x == x2 ? 0 : 0xAAAAAA, 0);
				}
			}
			if (s->neighboors[i] >= 0 && endx >= beginx && (head + MAXQUEUE + 1 - tail) % MAXQUEUE) {
				*head = (struct item) { s->neighboors[i], beginx, endx };
				if (++head == queue + MAXQUEUE)
					head = queue;
			}
		}
		++renderedSectors[now.sector];
	} while (head != tail);
	free(ytop);
	free(ybottom);
	free(renderedSectors);
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

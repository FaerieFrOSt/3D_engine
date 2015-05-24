#include "sector.h"
#include <stdlib.h>
#include <stdio.h>
#include "draw.h"

struct sector	*create_sector(float floor, float ceiling) {
	struct sector	*tmp = malloc(sizeof(*tmp));
	tmp->floor = floor;
	tmp->ceiling = ceiling;
	tmp->numVertices = 0;
	tmp->vertex = NULL;
	tmp->neighboors = 0;
	return tmp;
}

void	addVertice(struct sector *s, float x, float y) {
	struct vertice	*tmp = create_vertice(x, y);
	if (!s)
		return;
	++s->numVertices;
	s->vertex = realloc(s->vertex, s->numVertices * sizeof(*s->vertex));
	s->vertex[s->numVertices - 1] = tmp;
	s->neighboors = realloc(s->neighboors, s->numVertices * sizeof(*s->neighboors));
	s->neighboors[s->numVertices - 1] = -1;
}

void	delete_sector(struct sector **s) {
	if (!s || !*s)
		return;
	int i;
	for (i = 0; i < (*s)->numVertices; ++i)
		if ((*s)->vertex[i])
			free((*s)->vertex[i]);
	free(*s);
	*s = 0;
}

inline void	fill(SDL_Surface *s, unsigned int x, unsigned int y, const uint32_t color) {
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

void	drawSector(SDL_Surface *s, struct player *p, struct sector *se, uint8_t in) {
	int	i;
	int	x1, y1, x2, y2;
	int	tz1, tz2;
	for (i = 0; i < se->numVertices - 1; ++i) {
		x1 = se->vertex[i]->x;
		y1 = se->vertex[i]->y;
		x2 = se->vertex[i + 1]->x;
		y2 = se->vertex[i + 1]->y;
		// transform the vertexes
		x1 -= p->x;
		y1 -= p->y;
		x2 -= p->x;
		y2 -= p->y;
		// rotate vertexes around player
		tz1 = x1 * p->anglecos + y1 * p->anglesin;
		tz2 = x2 * p->anglecos + y2 * p->anglesin;
		x1 = x1 * p->anglesin - y1 * p->anglecos;
		x2 = x2 * p->anglesin - y2 * p->anglecos;
		// draw
		drawLine(s, s->w / 2 - x1, s->h / 2 - tz1, s->w / 2 - x2, s->h / 2 - tz2,
				se->neighboors[i] == -1 ? SDL_MapRGB(s->format, 255, 255, 255) :
				SDL_MapRGB(s->format, 255, 0, 0));
	}
	if (se->numVertices < 3)
		return;
	//fill algorithm
	if (in)
		fill(s, s->w / 2, s->h / 2, SDL_MapRGB(s->format, 0, 0, 50));
}


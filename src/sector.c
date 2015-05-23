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
	return tmp;
}

void	addVertice(struct sector *s, float x, float y) {
	struct vertice	*tmp = create_vertice(x, y);
	if (!s)
		return;
	++s->numVertices;
	s->vertex = realloc(s->vertex, s->numVertices * sizeof(*s->vertex));
	s->vertex[s->numVertices - 1] = tmp;
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

struct sector	*load_sector(struct sdl_data *data, const char *filename) {
	char	buf[256];
	snprintf(buf, sizeof buf, "%s/%s", data->folder, filename); // TODO : create a real ressource manager
	struct sector	*s = create_sector(0, 0);
	FILE	*fp = fopen(buf, "r");
	if (!fp) {
		perror(filename);
		exit(1);
	}
	char	word[256], *ptr;
	int	n;
	struct vertice	v;
	while (fgets(buf, sizeof buf, fp)) {
		switch (sscanf(ptr = buf, "%32s%n", word, &n) == 1 ? word[0] : 0) {
			case 'v': //vertice
				for (sscanf(ptr += n, "%f%n", &v.x, &n); sscanf(ptr += n, "%f%n", &v.y, &n) == 1;)
					addVertice(s, v.x, v.y);
				break;
			case 'f': //floor
				sscanf(ptr += n, "%f%n", &s->floor, &n);
				break;
			case 'c': //ceiling
				sscanf(ptr +=n, "%f%n", &s->ceiling, &n);
		}
	}
	fclose(fp);
	return s;
}

void	drawSector(SDL_Surface *s, struct player *p, struct sector *se) {
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
				SDL_MapRGB(s->format, 255, 0, 0));
	}
}

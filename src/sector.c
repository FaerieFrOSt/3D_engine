#include "sector.h"
#include <stdlib.h>

struct sector	*create_sector(int floor, int ceiling) {
	struct sector	*tmp = malloc(sizeof(*tmp));
	tmp->floor = floor;
	tmp->ceiling = ceiling;
	tmp->numVertices = 0;
	tmp->vertex = NULL;
	return tmp;
}

void	addVertice(struct sector *s, int x, int y) {
	struct vertice	*tmp = create_vertice(x, y);
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

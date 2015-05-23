#include "utils.h"

uint8_t	pointInSector(int x, int y, struct sector *s) {
	int	odd = 0;
	int	i, j = s->numVertices - 1;
	for (i = 0; i < s->numVertices; ++i) {
		if ((s->vertex[i]->y < y && s->vertex[j]->y >= y) ||
				(s->vertex[j]->y < y && s->vertex[i]->y >= y)) {
			if (s->vertex[i]->x + (y - s->vertex[i]->y) / (s->vertex[j]->y - s->vertex[i]->y)
					* (s->vertex[j]->x - s->vertex[i]->x) < x) {
				odd = 1 - odd;
			}
		}
		j = i;
	}
	return odd;
}

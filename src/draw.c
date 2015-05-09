#include "draw.h"

void	drawVLine(SDL_Surface *s, unsigned int x, unsigned int y1, unsigned y2, uint32_t color) {
	if (!s)
		return;
	if (x > s->w || y1 > s->h || y2 > s->h)
		return;
	if (y1 > y2) {
		unsigned int	tmp = y2;
		y2 = y1;
		y1 = tmp;
	}
	uint32_t	*pixels = s->pixels;
	unsigned int	i;
	for (i = y1; i < y2; ++i)
		pixels[i * s->w + x] = color;
}

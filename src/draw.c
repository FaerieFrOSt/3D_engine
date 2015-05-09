#include "draw.h"

void	drawVLine(SDL_Surface *s, unsigned int x, unsigned int y, uint32_t color) {
	if (!s)
		return;
	if (x > s->w || y > s->h)
		return;
	uint32_t	*pixels = s->pixels;
	pixels[y * s->w + x] = color;
}

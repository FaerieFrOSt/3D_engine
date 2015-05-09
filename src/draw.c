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
	for (i = y1; i <= y2; ++i)
		pixels[i * s->w + x] = color;
}

void	drawHLine(SDL_Surface *s, unsigned int x1, unsigned int x2, unsigned y, uint32_t color) {
	if (!s)
		return;
	if (x1 > s->w || x2 > s->w || y > s->h)
		return;
	if (x1 > x2) {
		unsigned int	tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	uint32_t	*pixels = s->pixels;
	unsigned int	i;
	for (i = x1; i <= x2; ++i)
		pixels[y * s->w + i] = color;
}

void	drawPixel(SDL_Surface *s, unsigned int x, unsigned int y, uint32_t color) {
	if (!s)
		return;
	if (x > s->w || y > s->h)
		return;
	uint32_t	*pixels = s->pixels;
	pixels[y * s->w + x] = color;
}

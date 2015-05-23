#include "draw.h"
#include "utils.h"

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

inline void	drawPixel(SDL_Surface *s, unsigned int x, unsigned int y, uint32_t color) {
	if (!s)
		return;
	if (x > s->w - 1 || y > s->h)
		return;
	uint32_t	*pixels = s->pixels;
	pixels[y * s->w + x] = color;
}

void	drawLine(SDL_Surface *s, int x1, int y1, int x2, int y2, uint32_t color) {
	int	dx = abs(x2 - x1);
	int	sx = x1 < x2 ? 1 : -1;
	int	dy = abs(y2 - y1);
	int	sy = y1 < y2 ? 1 : -1;
	int	err = (dx > dy ? dx : -dy) / 2, e2;
	int	i;
	// safe gard from infinite loop. Should be okay
	for (i = 0; i < 100000; ++i) {
		drawPixel(s, x1, y1, color);
		if (x1 == x2 && y1 == y2)
			break;
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x1 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y1 += sy;
		}
	}
}

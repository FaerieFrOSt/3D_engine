
#ifndef _3D_H_
# define _3D_H_

#include "sdl_init.h"
#include "map.h"

#define NEARZ		1e-4f
#define FARZ		5
#define NEARSIDE	1e-5f
#define FARSIDE		20.f

#define HFOV		(.73f * data->screen->h)
#define VFOV		(.2f * data->screen->h)

#define FNCross(x1, y1, x2, y2) ((x1) * (y2) - (x2) * (y1))
#define Intersect(x1, y1, x2, y2, x3, y3, x4, y4, x, y) do { \
				(x) = FNCross((x1), (y1), (x2), (y2)); \
				(y) = FNCross((x3), (y3), (x4), (y4)); \
				double	_det = FNCross((x1) - (x2), (y1) - (y2), (x3) - (x4), (y3) - (y4)); \
				(x) = FNCross((x), (x1) - (x2), (y), (x3) - (x4)) / _det; \
				(y) = FNCross((x), (y1) - (y2), (y), (y3) - (y4)) / _det; } while (0)

void	drawPerspective(struct sdl_data *data, struct map *map);
void	drawMinimap(struct sdl_data *data, struct map *map);

#endif /* !_3D_H_ */

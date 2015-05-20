#include "vertice.h"
#include <stdlib.h>

struct vertice	*create_vertice(float x, float y) {
	struct vertice	*tmp = malloc(sizeof(*tmp));
	tmp->x = x;
	tmp->y = y;
	return tmp;
}

void	delete_vertice(struct vertice **v) {
	if (!v || !*v)
		return;
	free(*v);
	*v = NULL;
}

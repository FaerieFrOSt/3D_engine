
#ifndef _VERTICE_H_
# define _VERTICE_H_

struct	vertice {
	float	x, y;
};

struct vertice	*create_vertice(float x, float y);
void			delete_vertice(struct vertice**);

#endif /* !_VERTICE_H_ */

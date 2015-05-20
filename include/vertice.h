
#ifndef _VERTICE_H_
# define _VERTICE_H_

struct	vertice {
	int	x, y;
};

struct vertice	*create_vertice(int x, int y);
void			delete_vertice(struct vertice**);

#endif /* !_VERTICE_H_ */

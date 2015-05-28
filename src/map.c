#include "map.h"
#include "utils.h"

struct map	*load_map(const struct sdl_data *data, const char *filename) {
	char	buf[256];
	uint8_t	debug = data->debug;
	snprintf(buf, sizeof buf, "%s/%s", data->folder, filename); // TODO : create a real ressource manager
	struct map *map = malloc(sizeof *map);
	map->sectors = 0;
	memset(&map->beginPosition, 0, sizeof map->beginPosition);
	map->nbSectors = 0;
	FILE	*fp = fopen(buf, "r");
	if (!fp) {
		perror(filename);
		exit(1);
	}
	char	word[256], *ptr;
	int	n;
	float	tmp;
	struct vertice	v;
	struct sector	*s = 0;
	while (fgets(buf, sizeof buf, fp)) {
		switch (sscanf(ptr = buf, "%32s%n", word, &n) == 1 ? word[0] : 0) {
			case 'p': //player
				sscanf(ptr += n, "%i%n", &map->beginPosition.x, &n);
				sscanf(ptr += n, "%i%n", &map->beginPosition.y, &n);
				sscanf(ptr += n, "%lf%n", &map->beginPosition.angle, &n);
				break;
			case 's': //sector
				if (s)
					addVertice(s, s->vertex[0]->x, s->vertex[0]->y);
				s = create_sector(0, 0);
				++map->nbSectors;
				map->sectors = realloc(map->sectors, map->nbSectors * sizeof *map->sectors);
				map->sectors[map->nbSectors - 1] = s;
			break;
			case 'v': //vertice
				for (sscanf(ptr += n, "%f%n", &v.x, &n); sscanf(ptr += n, "%f%n", &v.y, &n) == 1;)
					addVertice(s, v.x, v.y);
				break;
			case 'f': //floor
				if (!s)
					sscanf(ptr += n, "%f%n", &tmp, &n);
				sscanf(ptr += n, "%f%n", &s->floor, &n);
				break;
			case 'c': //ceiling
				if (!s)
					sscanf(ptr += n, "%f%n", &tmp, &n);
				sscanf(ptr +=n, "%f%n", &s->ceiling, &n);
				break;
	/*		case 'n': //neighboors
				if (!s)
					break;
				m = 0;
				for (;m < s->numVertices && sscanf(ptr += n, "%32s%n", word, &n) == 1;)
					s->neighboors[m++] = word[0] == 'x' ? -1 : atoi(word); */
		}
	}
	if (s)
		addVertice(s, s->vertex[0]->x, s->vertex[0]->y);
	// searching for neighbooring lines
	for (n = 0; n < map->nbSectors - 1; ++n) {
		int	m;
		for (m = n + 1; m < map->nbSectors; ++m) {
			int	k;
			for (k = 0; k < map->sectors[n]->numVertices - 1; ++k) {
				if (map->sectors[n]->neighboors[k] != -1)
						continue;
				int	l;
				for (l = 0; l < map->sectors[m]->numVertices - 1; ++l) {
					if (k >= map->sectors[n]->numVertices - 1)
						break;
					if (map->sectors[m]->neighboors[l] != -1 ||
						map->sectors[n]->neighboors[k] != -1)
						continue;
					struct vertice	p1 = *map->sectors[n]->vertex[k];
					struct vertice	p2 = *map->sectors[n]->vertex[k + 1];
					struct vertice	p3 = *map->sectors[m]->vertex[l];
					struct vertice	p4 = *map->sectors[m]->vertex[l + 1];
					double	a = p1.y - p2.y;
					double	b = p2.x - p1.x;
					double	c = (p1.x - p2.x) * p1.y + (p2.y - p1.y) * p1.x;
					#define EPSILON 1e-5
					if (fabs(a * p3.x + b * p3.y + c) < EPSILON &&
						fabs(a * p4.x + b * p4.y + c) < EPSILON) {
						#define dist(p)				((p).x * (p).x + (p).y + (p).y)
						#define distance(_p1, _p2)	(((_p1).x - (_p2).x) * ((_p1).x - (_p2).x) + ((_p1).y - (_p2).y) * ((_p1).y - (_p2).y))
						#define mn(_p1, _p2)		(dist(_p1) < dist(_p2) ? (_p1) : (_p2))
						#define mx(_p1, _p2)		(dist(_p1) > dist(_p2) ? (_p1) : (_p2))
						#define equals(_p1, _p2)	(fabs(_p1.x - _p2.x) < EPSILON && fabs(_p1.y - _p2.y) < EPSILON)
						if (dist(mx(p1, p2)) < dist(mn(p3, p4)) ||
							dist(mn(p1, p2)) > dist(mx(p3, p4)))
							continue;
						struct vertice	pn1 = mx(mn(p1, p2), mn(p3, p4));
						struct vertice	pn2 = mn(mx(p1, p2), mx(p3, p4));
						if (equals(pn1, pn2))
							continue;
						if (debug) {
							printf("Found pn1 : (%.2f %.2f), pn2 : (%.2f %.2f) between %d and %d\n", pn1.x, pn1.y, pn2.x, pn2.y, n, m);
							printf("p1 : (%.2f %.2f), p2 : (%.2f %.2f), p3 : (%.2f %.2f), p4 : (%.2f %.2f)\n", p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, p4.x, p4.y);
						}
						int	tmpK = k, tmpL = l;
						if (distance(pn1, p1) > distance(pn2, p1)) { // switch
							struct vertice	tmp = pn1;
							pn1 = pn2;
							pn2 = tmp;
						}
						if (!equals(pn1, p1) && !equals(pn1, p2)) { // add vertice in sector n
							if (debug)
								printf("1\n");
							insertVertice(map->sectors[n], pn1, ++k);
						}

						if (!equals(pn2, p2) && !equals(pn2, p1)) { // add vertice in sector n
							if (debug)
								printf("2\n");
							insertVertice(map->sectors[n], pn2,  ++k);
						}

						if (distance(pn1, p3) > distance(pn2, p3)) { // switch
							struct vertice	tmp = pn1;
							pn1 = pn2;
							pn2 = tmp;
						}
						if (!equals(pn1, p3) && !equals(pn1, p4)) { // add vertice in sector m
							if (debug)
								printf("3\n");
							insertVertice(map->sectors[m], pn1, ++l);
						}

						if (!equals(pn2, p4) && !equals(pn2, p3)) { // add vertice in sector m
							if (debug)
								printf("4\n");
							insertVertice(map->sectors[m], pn2, ++l);
						}

						if (k - tmpK > 1) {
							if (debug)
								printf("double k\n");
							map->sectors[n]->neighboors[k - 1] = m;
						} else if (k - tmpK > 0) {
							if (debug)
								printf("one k\n");
							map->sectors[n]->neighboors[k] = m;
						}
						if (l - tmpL > 1) {
							if (debug)
								printf("double l\n");
							map->sectors[m]->neighboors[l - 1] = n;
						} else if (l - tmpL > 0) {
							if (debug)
								printf("one l\n");
							map->sectors[m]->neighboors[l] = n;
						}

						if ((equals(pn1, p1) && equals(pn2, p2)) ||
							(equals(pn2, p1) && equals(pn2, p1)))
							map->sectors[n]->neighboors[k] = m;
						if ((equals(pn1, p3) && equals(pn2, p4)) ||
							(equals(pn2, p3) && equals(pn1, p4)))
							map->sectors[m]->neighboors[l] = n;
						if (!debug)
							continue;
						int q;
						printf("Sector : %d\n", m);
						for (q = 0; q < map->sectors[m]->numVertices; ++q)
							printf("(%.2f %.2f)\n", map->sectors[m]->vertex[q]->x, map->sectors[m]->vertex[q]->y);
					}
				}
			}
		}
	}
	// begin sector for player
	for (n = 0; n < map->nbSectors; ++n)
		if (pointInSector(map->beginPosition.x, map->beginPosition.y, map->sectors[n])) {
			map->beginPosition.sector = n;
			break;
		}
	fclose(fp);
	return map;
}

void	delete_map(struct map **m) {
	if (!m || !*m)
		return;
	int	i;
	for (i = 0; i < (*m)->nbSectors; ++i)
		if ((*m)->sectors[i])
			delete_sector(&(*m)->sectors[i]);
	free((*m)->sectors);
	free(*m);
	*m = 0;
	return;
}

void	drawMap(SDL_Surface *s, struct player *p, struct map *map, uint8_t debug) {
	int	i;
	for (i = 0; i < map->nbSectors; ++i)
		drawSector(s, p, map->sectors[i], debug);
}

#include "map.h"
#include "utils.h"

struct map	*load_map(const struct sdl_data *data, const char *filename) {
	char	buf[256];
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
	struct sector	*s;
	while (fgets(buf, sizeof buf, fp)) {
		switch (sscanf(ptr = buf, "%32s%n", word, &n) == 1 ? word[0] : 0) {
			case 'p': //player
				sscanf(ptr += n, "%i%n", &map->beginPosition.x, &n);
				sscanf(ptr += n, "%i%n", &map->beginPosition.y, &n);
				sscanf(ptr += n, "%lf%n", &map->beginPosition.angle, &n);
				break;
			case 's': //sector
				s = create_sector(0, 0);
				++map->nbSectors;
				map->sectors = realloc(map->sectors, map->nbSectors * sizeof *map->sectors);
				map->sectors[map->nbSectors - 1] = s;
			break;
			case 'v': //vertice
				for (sscanf(ptr += n, "%f%n", &v.x, &n); sscanf(ptr += n, "%f%n", &v.y, &n) == 1;)
					addVertice(s, v.x, v.y); // TODO : verify closed sector
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
		}
	}
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

void	drawMap(SDL_Surface *s, struct player *p, struct map *map) {
	int	i;
	for (i = 0; i < map->nbSectors; ++i)
		drawSector(s, p, map->sectors[i], p->sector == i);
}

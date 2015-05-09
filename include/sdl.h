#ifndef _SDL_H_
# define _SDL_H_

#include <SDL2/SDL.h>

#define W 640
#define H 480

struct sdl_data {
	SDL_Window		*win;
	SDL_Renderer	*ren;
	SDL_Surface		*screen;
	SDL_Texture		*tex;
	struct {
		char		key[SDL_NUM_SCANCODES];
		int			quit;
	} events;
};

struct sdl_data	*init(const char *title);
void			end(struct sdl_data **);
void			print(struct sdl_data *);

void			updateEvents(struct sdl_data *);

#endif /* !_SDL_H_ */

#include "sdl.h"

struct sdl_data	*init(const char *title) {
	struct sdl_data	*data = malloc(sizeof(struct sdl_data));
	if (!data)
		exit(1); // TODO : print a message
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init error : %s\n", SDL_GetError());
		exit(1);
	}
	data->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
						SDL_WINDOWPOS_CENTERED, W, H, 0);
	if (!data->win) {
		printf("error creating window : %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
	data->ren = SDL_CreateRenderer(data->win, -1, SDL_RENDERER_ACCELERATED |
							SDL_RENDERER_PRESENTVSYNC);
	if (!data->ren) {
		printf("error creating renderer : %s\n", SDL_GetError());
		SDL_DestroyWindow(data->win);
		SDL_Quit();
		exit(1);
	}
	SDL_SetRenderDrawColor(data->ren, 0, 0, 0, 255);
	data->screen = SDL_CreateRGBSurface(0, W, H, 32, 0, 0, 0, 0);
	SDL_FillRect(data->screen, NULL, SDL_MapRGB(data->screen->format, 0, 0, 0));
	data->tex = SDL_CreateTexture(data->ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, W, H);

	memset(data->events.key, 0, sizeof(data->events.key));
	data->events.quit = 0;

	// the window grabs the mouse
	SDL_SetRelativeMouseMode(SDL_TRUE);
	return data;
}

void	end(struct sdl_data **data) {
	if (!data || !*data)
		return;
	if ((*data)->ren)
		SDL_DestroyRenderer((*data)->ren);
	if ((*data)->win)
		SDL_DestroyWindow((*data)->win);
	if ((*data)->screen)
		SDL_FreeSurface((*data)->screen);
	SDL_Quit();
	free(*data);
	*data = NULL;
}

void	print(struct sdl_data *data) {
		SDL_UpdateTexture(data->tex, NULL, data->screen->pixels, W * sizeof(*data->screen->pixels));
		SDL_RenderClear(data->ren);
		SDL_RenderCopy(data->ren, data->tex, NULL, NULL);
		SDL_RenderPresent(data->ren);
}

void	updateEvents(struct sdl_data *data) {
	SDL_Event	e;
	while (SDL_PollEvent(&e))
		switch (e.type) {
			case SDL_KEYDOWN:
				data->events.key[e.key.keysym.sym] = 1;
				break;
			case SDL_KEYUP:
				data->events.key[e.key.keysym.sym] = 0;
				break;
			case SDL_QUIT:
				data->events.quit = 1;
				break;
			default:
				break;
		}
}

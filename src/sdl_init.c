#include "sdl_init.h"

struct sdl_data	*init(const char *folder, const char *title) {
	struct sdl_data	*data = malloc(sizeof(struct sdl_data));
	if (!data)
		exit(1); // TODO : print a message
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init error : %s\n", SDL_GetError());
		exit(1);
	}
	if (TTF_Init() == -1) {
		printf("TTF_Init error : %s\n", TTF_GetError());
		SDL_Quit();
		exit(1);
	}
	data->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
						SDL_WINDOWPOS_CENTERED, W, H, 0);
	if (!data->win) {
		printf("error creating window : %s\n", SDL_GetError());
		SDL_Quit();
		exit(1);
	}
	data->ren = SDL_CreateRenderer(data->win, -1, SDL_RENDERER_ACCELERATED /* |
							SDL_RENDERER_PRESENTVSYNC */);
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

	// set working folder
	size_t	len = strlen(folder);
	while (folder[--len] != '/');
	data->folder = malloc((len + 1) * sizeof(*data->folder));
	strncpy(data->folder, folder, len);
	data->folder[len] = '\0';

	// Events
	memset(data->events.key, 0, sizeof(data->events.key));
	data->events.quit = 0;

	// FPS
	data->fps.countedFrames = 0;
	data->fps.fps = 0;
	data->fps.startTime = SDL_GetTicks();
	data->fps.print = 0;
	data->fps.capFPS = 0;
	data->fps.frameTicks = 0;
	data->fps.ticksPerFrame = 0;

	// Font
	// TODO : create a real font manager
	char	buf[256];
	snprintf(buf, sizeof buf, "%s/font.ttf", data->folder);
	data->font.font = TTF_OpenFont(buf, 10);
	data->font.color.r = 255;
	data->font.color.g = 255;
	data->font.color.b = 255;
	data->font.color.a = 0;
	data->font.pos.x = W - 35;
	data->font.pos.y = 10;
	if (!data->font.font) {
		printf("TTF error : %s\n", TTF_GetError());
		end(&data);
		exit(1);
	}

	// the window grabs the mouse
	SDL_SetRelativeMouseMode(SDL_TRUE);
	return data;
}

void	end(struct sdl_data **data) {
	if (!data || !*data)
		return;
	if ((*data)->folder)
		free((*data)->folder);
	if ((*data)->ren)
		SDL_DestroyRenderer((*data)->ren);
	if ((*data)->win)
		SDL_DestroyWindow((*data)->win);
	if ((*data)->screen)
		SDL_FreeSurface((*data)->screen);
	if ((*data)->font.font)
		TTF_CloseFont((*data)->font.font);
	TTF_Quit();
	SDL_Quit();
	free(*data);
	*data = NULL;
}

void	display(struct sdl_data *data) {
		data->fps.fps = data->fps.countedFrames / ((SDL_GetTicks() - data->fps.startTime) / 1000.f);
		SDL_Texture	*text = NULL;
		if (data->fps.print)
		{
			char	buf[16];
			memset(buf, 0, sizeof buf);
			snprintf(buf, sizeof buf, "%.2f", data->fps.fps);
			SDL_Surface	*tmp = TTF_RenderText_Solid(data->font.font, buf, data->font.color);
			text = SDL_CreateTextureFromSurface(data->ren, tmp);
			data->font.pos.w = tmp->w;
			data->font.pos.h = tmp->h;
			data->font.pos.x = W - tmp->w - 5;
			SDL_FreeSurface(tmp);
		}
		SDL_UpdateTexture(data->tex, NULL, data->screen->pixels, data->screen->pitch);
		SDL_RenderClear(data->ren);
		SDL_RenderCopy(data->ren, data->tex, NULL, NULL);
		if (text)
			SDL_RenderCopy(data->ren, text, NULL, &data->font.pos);
		SDL_RenderPresent(data->ren);
		++data->fps.countedFrames;
		// cap FPS if needed
		uint32_t	tmp = SDL_GetTicks();
		if (data->fps.capFPS && tmp - data->fps.frameTicks < SCREEN_TICKS_PER_FRAME) {
			SDL_Delay(SCREEN_TICKS_PER_FRAME - (tmp - data->fps.frameTicks));
		}
		data->fps.ticksPerFrame = tmp - data->fps.frameTicks;
		data->fps.frameTicks = SDL_GetTicks();
}

void	updateEvents(struct sdl_data *data) {
	SDL_Event	e;
	while (SDL_PollEvent(&e))
		switch (e.type) {
			case SDL_KEYDOWN:
				data->events.key[e.key.keysym.scancode] = 1;
				break;
			case SDL_KEYUP:
				data->events.key[e.key.keysym.scancode] = 0;
				break;
			case SDL_QUIT:
				data->events.quit = 1;
				break;
			default:
				break;
		}
}

inline int	keyPressed(struct sdl_data *data, char key) {
	return data->events.key[(int)key];
}

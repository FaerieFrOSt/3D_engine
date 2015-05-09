#include <SDL2/SDL.h>
#include <stdio.h>

#define W 640
#define H 480

int	main() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL_Init error : %s\n", SDL_GetError());
		return 1;
	}
	SDL_Window	*win = SDL_CreateWindow("Hello world", SDL_WINDOWPOS_CENTERED,
						SDL_WINDOWPOS_CENTERED, W, H, 0);
	if (!win) {
		printf("error creating window : %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_Renderer	*ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED |
							SDL_RENDERER_PRESENTVSYNC);
	if (!ren) {
		printf("error creating renderer : %s\n", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_Surface *screen = SDL_CreateRGBSurface(0, W, H, 32, 0, 0, 0, 0);
	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 0, 0));
	SDL_Texture	*tex = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, W, H);
	int	quit = 0;
	while (!quit) {
		SDL_Event	e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = 1;
			else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
				switch (e.key.keysym.sym) {
					case 'z':
						break;
					case 'q':
						break;
					case 's':
						break;
					case 'd':
						break;
					case 'a':
					case SDLK_ESCAPE:
						quit = 1;
						break;
					default:
						break;
				}
		}
		SDL_UpdateTexture(tex, NULL, screen->pixels, W * sizeof(*screen->pixels));
		SDL_RenderClear(ren);
		SDL_RenderCopy(ren, tex, NULL, NULL);
		SDL_RenderPresent(ren);
		SDL_Delay(10);
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

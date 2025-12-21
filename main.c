#include<stdio.h>
#include<SDL2/SDL.h>
#include<assert.h>

#define BOOL u32
#define TRUE 1
#define FALSE 0

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 576

typedef Uint32 u32;
typedef Uint64 u64;
typedef Sint32 s32;
typedef Sint64 s64;

typedef struct {
	int x;
	int y;
	int w;
	int h;
} rect_t;

void FillRect(rect_t rect, u32 pixel_color, u32 *screen_pixels) {
	for (int row = 0; row < rect.h; row++) {
		for (int col = 0; col < rect.w; col++) {
			screen_pixels[(row+rect.y)*SCREEN_WIDTH + col + rect.x] = pixel_color;
		}
	}
}


int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_Window *win = SDL_CreateWindow("Spell Checker",
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									SCREEN_WIDTH,
									SCREEN_HEIGHT,
									0
								);
	SDL_Renderer *renderer = SDL_CreateRenderer(win, 0, SDL_RENDERER_SOFTWARE);
	assert(renderer);
	
	SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);

	SDL_Texture *screen = SDL_CreateTexture(renderer,
										format->format,
										SDL_TEXTUREACCESS_STREAMING,
										SCREEN_WIDTH,
										SCREEN_HEIGHT
										);
	assert(screen);

	u32 *screen_pixels = (u32*) calloc(SCREEN_WIDTH * SCREEN_HEIGHT, sizeof(u32));
	assert(screen_pixels);

	rect_t square = {100, 100, 25, 30};

	u32 pixel_color = SDL_MapRGB(format, 255, 0, 0);
	square.x = (SCREEN_WIDTH - square.w)/2;
	square.y = (SCREEN_HEIGHT - square.h)/2;
	FillRect(square, pixel_color, screen_pixels);

	BOOL move_up = FALSE;
	BOOL move_down = FALSE;
	BOOL move_right = FALSE;
	BOOL move_left = FALSE; 
	
	BOOL done = FALSE;		

	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				done = TRUE;
				break;
			}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym){
					case SDLK_ESCAPE:
						done = TRUE;
						break;
					case SDLK_j:
						move_down = TRUE;
						break;
					case SDLK_k: 
						move_up = TRUE;
						break;
					case SDLK_l:
						move_right = TRUE; 
						break;
					case SDLK_h:
						move_left = TRUE;
						break;
				}
			}
			if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym){
					case SDLK_j:
						move_down = FALSE;
						break;
					case SDLK_k: 
						move_up = FALSE;
						break;
					case SDLK_l:
						move_right = FALSE; 
						break;
					case SDLK_h:
						move_left = FALSE;
						break;
				}
			}
		}
		if (move_up) square.y += 1;
		if (move_down) square.y -= 1;
		if (move_left) square.x -= 1;
		if (move_right) square.x += 1;

		FillRect(square, pixel_color, screen_pixels);
		SDL_UpdateTexture(screen, NULL, screen_pixels, SCREEN_WIDTH * sizeof(u32));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, screen, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_Delay(50);
	} 
	SDL_Quit(); 
	return 0; 
}
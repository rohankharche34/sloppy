#include<stdio.h>
#include<SDL2/SDL.h>
#include<assert.h>
#include<SDL_ttf.h>

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
	s32 x;
	s32 y;
	s32 w;
	s32 h;
} rect_t;

void FillRect(rect_t rect, u32 pixel_color, u32 *screen_pixels) {
	for (s32 row = 0; row < rect.h; row++) {
		for (s32 col = 0; col < rect.w; col++) {
			s32 vertical = row + rect.y;
			s32 horizon = col + rect.x;

			if (vertical < 0 || vertical >= SCREEN_HEIGHT ||
				horizon  < 0 || horizon  >= SCREEN_WIDTH)
    			continue;
			
			screen_pixels[(vertical)*SCREEN_WIDTH + horizon] = pixel_color;
		}
	}
}

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;
	SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

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

	rect_t cursor = {0, 0, 10, 20};

	u32 pixel_color = SDL_MapRGB(format, 128, 128, 128);
	//cursor.x = (SCREEN_WIDTH - cursor.w)/2;
	//cursor.y = (SCREEN_HEIGHT - cursor.h)/2;
	FillRect(cursor, pixel_color, screen_pixels);

	TTF_Font *font = TTF_OpenFont("fonts/JetBrainsMono-2.304/ttf/JetBrainsMonoNL-Regular.ttf", 14);
	assert(font);

	SDL_Color textcolor = {255, 255, 255, 255};
	SDL_Surface *textsurface = NULL;
	SDL_Texture *texttexture = NULL;	

	BOOL move_up = FALSE;
	BOOL move_down = FALSE;
	BOOL move_right = FALSE;
	BOOL move_left = FALSE; 
	
	char ascii = '\0';
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
					case SDLK_DOWN:
						move_down = TRUE;
						break;
					case SDLK_UP: 
						move_up = TRUE;
						break;
					case SDLK_RIGHT:
						move_right = TRUE; 
						break;
					case SDLK_LEFT:
						move_left = TRUE;
						break;
					default:
						ascii = (char) event.key.keysym.sym;
						break;
				}
			}
			if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym){
					case SDLK_DOWN:
						move_down = FALSE;
						break;
					case SDLK_UP: 
						move_up = FALSE;
						break;
					case SDLK_RIGHT:
						move_right = FALSE; 
						break;
					case SDLK_LEFT:
						move_left = FALSE;
						break;
				}
			}
		}
		memset(screen_pixels, 0, SCREEN_WIDTH*SCREEN_HEIGHT*sizeof(u32));

		if (move_up) cursor.y -= 5;
		if (move_down) cursor.y += 5;
		if (move_left) cursor.x -= 5;
		if (move_right) cursor.x += 5;

		FillRect(cursor, pixel_color, screen_pixels);	

		SDL_UpdateTexture(screen, NULL, screen_pixels, SCREEN_WIDTH * sizeof(u32));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, screen, NULL, NULL);

		if (ascii != '\0') {
			char msg[2] = { ascii, '\0' };
			textsurface = TTF_RenderUTF8_Blended(font, msg, textcolor);
			texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);

			SDL_Rect dstRect = {1, 1, textsurface->w, textsurface->h};
			SDL_RenderCopy(renderer, texttexture, NULL, &dstRect);
		}
		SDL_RenderPresent(renderer);
		SDL_Delay(50);

	}
	SDL_DestroyTexture(texttexture);
	SDL_DestroyTexture(screen);	
	SDL_FreeSurface(textsurface);	
	SDL_DestroyRenderer(renderer);
	TTF_Quit();  
	SDL_Quit(); 
	return 0; 
}
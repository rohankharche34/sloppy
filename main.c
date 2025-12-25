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

	TTF_Font *font = TTF_OpenFont("fonts/JetBrainsMono-2.304/ttf/JetBrainsMonoNL-Regular.ttf", 14);
	assert(font);

    s32 minx, maxx, miny, maxy, advance;
    TTF_GlyphMetrics(font, 'M', &minx, &maxx, &miny, &maxy, &advance);

    s32 char_width  = advance;
    s32 char_height = TTF_FontLineSkip(font);
	
	SDL_StartTextInput();

	char text_buffer[1024] = "";
	BOOL text_changed = TRUE;
	s32 cursor_col = 0;

	SDL_Surface *textsurface = NULL;
	SDL_Texture *texttexture = NULL;

	BOOL done = FALSE;	

	while (!done) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				done = TRUE;
				break;
			}
			if (event.type == SDL_TEXTINPUT) {
				strcat(text_buffer, event.text.text);
				cursor_col++;
				text_changed = TRUE;
			}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym){
					case SDLK_ESCAPE:
						done = TRUE;
						break;
					case SDLK_RIGHT:
						 if (cursor_col < (int)strlen(text_buffer))
        					cursor_col++;
						break;
					case SDLK_LEFT:
						if (cursor_col > 0)
        					cursor_col--;
						break;
					case SDLK_BACKSPACE:
						size_t len = strlen(text_buffer);
						if (len > 0) {
							text_buffer[len-1] = '\0';
							cursor_col--;
							text_changed = TRUE;
						}
					}
				}
			}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		SDL_Color textcolor = {255, 255, 255, 255};
		if (text_changed) {
			if (texttexture) SDL_DestroyTexture(texttexture);
			if (textsurface) SDL_FreeSurface(textsurface);
			
			textsurface = TTF_RenderUTF8_Blended(
				font,
				text_buffer,
				textcolor
			);
			texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
			text_changed = FALSE;
		}
		if (texttexture && textsurface) {
			SDL_Rect textRect = {0, 0, textsurface->w, textsurface->h};
			SDL_RenderCopy(renderer, texttexture, NULL, &textRect);
		}
		
		SDL_Rect cursor = {
			0 + cursor_col * char_width,
			0,
			2,
			char_height
		};
		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
		SDL_RenderFillRect(renderer, &cursor);

		SDL_RenderPresent(renderer);
		SDL_Delay(50);
	}
	SDL_StopTextInput();
	SDL_DestroyTexture(texttexture);
	SDL_FreeSurface(textsurface);	
	SDL_DestroyRenderer(renderer);
	TTF_Quit();  
	SDL_Quit(); 
	return 0; 
}
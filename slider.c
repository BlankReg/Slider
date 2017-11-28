#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SDL_MAIN_HANDLED	1
#include "SDL2\SDL.h"

SDL_Window	*window;
SDL_Surface *surface;
SDL_Surface *image;
int movestaken = 0;

enum {UP, DOWN, LEFT, RIGHT};

#define spritewidth		52
#define spriteheight	52

char board[5][5] = {
	{'A','B','C','D','E'},
	{'F','G','H','I','J'},
	{'K','L','M','N','O'},
	{'P','Q','R','S','T'},
	{'U','V','W','X',' '}
	};

void move(int *px, int *py, int direction) {
	char c = 0;
	int lx = *px, ly = *py;

	if (direction == LEFT)	if (*px > 0) {(*px)--; movestaken++;}
	if (direction == RIGHT)	if (*px < 4) {(*px)++; movestaken++;}
	if (direction == DOWN) 	if (*py < 4) {(*py)++; movestaken++;}
	if (direction == UP)	if (*py > 0) {(*py)--; movestaken++;}

	c = board[*py][*px];
	board[*py][*px] = ' ';
	board[ly][lx] = c;
}

void shuffle(int *px, int *py) {
	int direction = 0;
	SDL_bool okaytomove = SDL_FALSE;
	int lastdirection = 0;
	int count = 0;
	srand (time(NULL));

	while (count < 500) {
		okaytomove = 0;
		while(okaytomove == SDL_FALSE)
		{
			direction = rand() % 4;
			if (direction == UP		&& *py != 0 && lastdirection != DOWN)	okaytomove = SDL_TRUE;
			if (direction == DOWN	&& *py != 4 && lastdirection != UP)		okaytomove = SDL_TRUE;
			if (direction == LEFT	&& *px != 0 && lastdirection != RIGHT)	okaytomove = SDL_TRUE;
			if (direction == RIGHT	&& *px != 4 && lastdirection != LEFT)	okaytomove = SDL_TRUE;
			SDL_UpdateWindowSurface(window);
		}

		if (direction == UP )	move(&(*px), &(*py), UP);
		if (direction == DOWN)	move(&(*px), &(*py), DOWN);
		if (direction == LEFT)	move(&(*px), &(*py), LEFT);
		if (direction == RIGHT) move(&(*px), &(*py), RIGHT);

		lastdirection = direction;
		count++;
	}
}

void drawboard(void) {
	SDL_Rect	rect;
	SDL_Rect 	spriterect;
	int c = 0;

	rect.w = spritewidth;
	rect.h = spriteheight;

	spriterect.y = 0;
	spriterect.w = spritewidth;
	spriterect.h = spriteheight;

	for (int x = 0; x < 5; x++) {
		for (int y = 0; y < 5; y++) {
			if (board[x][y] == 32) c = 1248; else c = ((board[x][y]) - 65) * spritewidth;
			rect.x = y * spriteheight;
			rect.y = x * spritewidth;
			spriterect.x = c;
			SDL_BlitSurface(image, &spriterect, surface, &rect);
		}
	}
	SDL_UpdateWindowSurface(window);
}

SDL_bool solved(int *px, int *py) {
	int count = 65;
	for (int x = 0; x < 5; x++)
		for (int y = 0; y < 5; y++) if (board[x][y] == count) count++;
	if (count == 89 && *px == 4 && *py==4) return SDL_TRUE; else return SDL_FALSE;
}

int solvedmessage(void) {
	int buttonid = 0;
	char buffer[512] = {'\0'};

	const SDL_MessageBoxButtonData buttons[] = {
		{0, 0, "No" }, { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes" },
	};

	sprintf(buffer,"Well done, you took %d moves\nWould you like another game?", movestaken);
	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION,
		NULL,
		"Congratulations ",
		buffer,
		SDL_arraysize(buttons),
		buttons,
		NULL
	};

	SDL_ShowMessageBox(&messageboxdata, &buttonid);
	return buttonid;
}

int main(int argc, char* argv[])
{
	int px = 4, py = 4;
	char buffer[80] = {'\0'};
	SDL_Event	event;

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow( "Slider", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, 260, 260, SDL_WINDOW_OPENGL );

	surface = SDL_GetWindowSurface(window);
	image = SDL_LoadBMP( "tiles.bmp" );

	shuffle(&px, &py);
	movestaken = 0;

	for(;;) {
		if (SDL_PollEvent(&event) == 0 ) {
			drawboard();
			if (solved(&px, &py) == SDL_TRUE) {
				if (solvedmessage() == 1) {
					shuffle(&px, &py);
					movestaken = 0;
				}
				else break;
 			}
		}
		else {
			sprintf(buffer,"Moves : %.3d", movestaken);
			SDL_SetWindowTitle(window,buffer);

			if (event.type == SDL_KEYDOWN) {
      			switch( event.key.keysym.sym ) {
		        	case SDLK_LEFT:
        				move(&px, &py, LEFT);
        				break;
					case SDLK_RIGHT:
						move(&px, &py, RIGHT);
		        	    break;
					case SDLK_UP:
						move(&px, &py, UP);
						break;
					case SDLK_DOWN:
						move(&px, &py, DOWN);
						break;
					case SDLK_ESCAPE:
						event.type = SDL_QUIT;
						break;
					default:
						break;
				}
			}

			if (event.type == SDL_QUIT) break;
		}
	}

	SDL_DestroyWindow(window);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(image);
	SDL_Quit();
	return 0;
}

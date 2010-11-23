#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_framerate.h"

void drawFrame(SDL_Surface* screen, int radius) {
	static const int x = 320;
	static const int y = 240;
	static const Uint32 borderColour = 0x000000FF;
	static const Uint32 fillColour = 0xAA00AAFF;
	static const Uint32 backgroundColour = 0xAA44AAFF;

	/* For no reason the filledRectangle is called box */
	boxColor(screen, 0, 0, 640, 480, backgroundColour);

	filledCircleColor(screen, x, y, radius + 4, borderColour);
	filledCircleColor(screen, x, y, radius, fillColour);
}

#define FPS 60

int main (int argc, char** argv) {
	SDL_Surface *screen;
	FPSmanager fpsm;
	long i;

	SDL_Init(SDL_INIT_VIDEO);

	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
	SDL_initFramerate(&fpsm);
	SDL_setFramerate(&fpsm, FPS);

	for(i=0; i <= FPS * 4; i++) {
		drawFrame(screen, i);
		SDL_Flip(screen);
		SDL_framerateDelay(&fpsm);
	}

	SDL_Quit();
	return 0;
}


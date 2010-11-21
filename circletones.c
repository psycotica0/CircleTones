#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"

int main (int argc, char** argv) {
	SDL_Surface *screen;
	int i;

	SDL_Init(SDL_INIT_VIDEO);

	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
	/* I was trying to figure out how to make a line with multiple pixels. Then I realized I just make a bigger circle it sits on... */
	filledCircleRGBA(screen, 300, 300, 53, 0x00, 0xFF, 0x00, 0xFF);
	filledCircleRGBA(screen, 300, 300, 50, 0xFF, 0x00, 0xFF, 0xFF);
	SDL_Flip(screen);
	SDL_Delay(2000);

	SDL_Quit();
	return 0;
}


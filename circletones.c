#include <stdlib.h>
#include <stdio.h>

#include <math.h>
#include "SDL/SDL.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_framerate.h"

typedef struct Circle {
	int x;
	int y;
	int radius;
	int borderWidth;
	int rate;
	Uint32 borderColour;
	Uint32 fillColour;
} Circle;

typedef struct CircleNode {
	Circle* circle;
	struct CircleNode* next;
} CircleNode;

typedef struct CircleList {
	int size;
	struct CircleNode* head;
} CircleList;

CircleNode* newCircle(CircleList* list, int x, int y) {
	CircleNode* temp;

	temp = malloc(sizeof(CircleNode));
	if (temp == NULL)
		return NULL;

	temp->circle = malloc(sizeof(Circle));
	if (temp->circle == NULL) {
		free(temp);
		return NULL;
	}

	temp->circle->x = x;
	temp->circle->y = y;

	temp->circle->rate = 1;
	temp->circle->radius = 1;
	temp->circle->borderWidth = 2;
	temp->circle->borderColour = 0x000000FF;
	temp->circle->fillColour = 0xAA00AAFF;

	temp->next = list->head;
	list->head = temp;

	list->size++;

	return temp;
}

CircleList* initializeCircleList() {
	CircleList* temp;

	temp = malloc(sizeof(CircleList));
	temp->head = NULL;
	temp->size = 0;

	return temp;
}

/* This function returns the absolute distance between the centres of circle1 and circle2 */
int calculateAbsDistance(Circle* circle1, Circle* circle2) {
	int x = circle1->x - circle2->x;
	int y = circle1->y - circle2->y;
	return (int)sqrt(x*x + y*y);
}

void doCollisionNode(CircleNode* head) {
	CircleNode* current;
	for (current = head->next; current != NULL; current=current->next) {
		if (calculateAbsDistance(head->circle, current->circle) <= (head->circle->radius + current->circle->radius)) {
			/* They're hitting */
			if (head->circle->rate >= 0) {
				/* If it's expanding, make it shrink */
				head->circle->rate *= -1;
			}

			if (current->circle->rate >= 0) {
				/* Make it shrink too */
				current->circle->rate *= -1;
			}
		}
	}
}

void doRebound(Circle* current) {
	if (current->radius <= 1 && current->rate <= 0) {
		current->rate *= -1;
	}
}

void drawCircle(SDL_Surface* screen, Circle* circle) {
	/* Border */
	if (circle->borderWidth > 0)
		filledCircleColor(screen, circle->x, circle->y, circle->radius + circle->borderWidth, circle->borderColour);
	/* Fill */
	filledCircleColor(screen, circle->x, circle->y, circle->radius, circle->fillColour);
}

void updateCircle(Circle* circle) {
	circle->radius += circle->rate;
}

void drawFrame(SDL_Surface* screen, CircleList* list) {
	CircleNode* current;
	Uint32 backgroundColour = 0xAA44AAFF;
	/* For no reason the filledRectangle is called box */
	/* Anyway, clear the screen */
	boxColor(screen, 0, 0, 640, 480, backgroundColour);
	for (current = list->head; current != NULL; current = current->next) {
		drawCircle(screen, current->circle);
		doRebound(current->circle);
		doCollisionNode(current);
		updateCircle(current->circle);
	}
}

int handleEvents(CircleList* list) {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT:
				return 1;
				break;
			case SDL_MOUSEBUTTONDOWN:
				newCircle(list, event.button.x, event.button.y);
				break;
		}
	}
	return 0;
}

#define FPS 60

int main (int argc, char** argv) {
	SDL_Surface *screen;
	FPSmanager fpsm;
	CircleList* list;
	int stopping = 0;

	SDL_Init(SDL_INIT_VIDEO);
	list = initializeCircleList();

	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
	SDL_initFramerate(&fpsm);
	SDL_setFramerate(&fpsm, FPS);

	while (1) {
		if (handleEvents(list)) /* handleEvents returns something other than 0 when we should shutdown */
			break;
		drawFrame(screen, list);
		SDL_Flip(screen);
		SDL_framerateDelay(&fpsm);
	}

	SDL_Quit();
	return 0;
}


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
} Circle;

typedef struct CircleNode {
	Circle* circle;
	struct CircleNode* next;
} CircleNode;

typedef struct CircleList {
	int size;
	struct CircleNode* head;
} CircleList;

Circle* newCircle(int x, int y) {
	Circle* temp;

	temp = malloc(sizeof(Circle));
	if (temp == NULL) {
		free(temp);
		return NULL;
	}

	temp->x = x;
	temp->y = y;

	temp->rate = 1;
	temp->radius = 1;
	temp->borderWidth = 2;
	temp->borderColour = 0xFFFFFFFF;

	return temp;
}

/* This function puts the given circle into a node into the list and returns the circle again */
Circle* pushCircle(CircleList* list, Circle* circle) {
	CircleNode* node;

	if (circle == NULL)
		return NULL;

	node = malloc(sizeof(CircleNode));

	if (node == NULL)
		return NULL;

	node->circle = circle;

	node->next = list->head;
	list->head = node;

	list->size++;

	return circle;
}

CircleList* newCircleList() {
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
	aacircleColor(screen, circle->x, circle->y, circle->radius, circle->borderColour);
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
				pushCircle(list, newCircle(event.button.x, event.button.y));
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
	list = newCircleList();

	screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
	SDL_initFramerate(&fpsm);
	SDL_setFramerate(&fpsm, FPS);

	SDL_WM_SetCaption("CircleTones", NULL);

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


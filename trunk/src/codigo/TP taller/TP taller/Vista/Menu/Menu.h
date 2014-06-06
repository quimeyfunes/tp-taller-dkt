#ifndef __MENU_H__
#define __MENU_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "../Dibujables/CartelDibujable.h"

class Menu {

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	list<DibujableTextura*>* listaDibujables;
	int ancho;
	int alto;
public:
	Menu(SDL_Window* window, SDL_Renderer* renderer);
	Menu();
	~Menu();

	void dibujar();
};

	
#endif
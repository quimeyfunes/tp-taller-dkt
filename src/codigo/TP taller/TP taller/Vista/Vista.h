#ifndef __VISTA_H__
#define __VISTA_H__

#include "FiguraDibujable.h"
#include <list>

using namespace std;

class Vista
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	list<Dibujable*>* listaDibujables;
	
public:

	Vista();
	~Vista();
	SDL_Renderer* getRenderer();
	Dibujable* crearDibujable(SDL_Rect rect, string pathImagen);
	FiguraDibujable* crearFiguraDibujable(SDL_Rect rect, string pathImagen);
	void agregarDibujable(Dibujable* dibujable);
	list<Dibujable*>* getListaDibujables();
	void Dibujar();
	
};

#endif
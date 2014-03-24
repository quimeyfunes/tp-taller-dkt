#include "Vista.h"

Vista::Vista(){
	SDL_Init( SDL_INIT_EVERYTHING );
	this->window = SDL_CreateWindow("Worms!", 50, 50, 400, 400,  SDL_WINDOW_SHOWN );
	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	this->listaDibujables = new list<Dibujable*>;
}

Vista::~Vista() {
	delete this->window;
	delete this->renderer;
	delete this->listaDibujables;
}
SDL_Renderer* Vista::getRenderer() {
	return this->renderer;
}

Dibujable* Vista::crearDibujable(SDL_Rect rect, string pathImagen) {
	Dibujable* dib = new Dibujable(this->renderer, rect, pathImagen);
	this->agregarDibujable(dib);
	return dib;
}

FiguraDibujable* Vista::crearFiguraDibujable(SDL_Rect rect, string pathImagen) {
	FiguraDibujable* dib = new FiguraDibujable(this->renderer, rect, pathImagen);
	this->agregarDibujable(dib);
	return dib;
}

void Vista::agregarDibujable(Dibujable* dibujable) {
	this->listaDibujables->push_back(dibujable);
}

list<Dibujable*>* Vista::getListaDibujables(){
	return this->listaDibujables;
	
}

void Vista::Dibujar(){
	SDL_RenderClear(this->renderer);
	for (list<Dibujable*>::iterator it = this->listaDibujables->begin(); it != this->listaDibujables->end(); it++) {
		SDL_RenderCopyEx(this->renderer,(*it)->getImagen(),NULL, &(*it)->getRect(),(*it)->getAngulo(),NULL,SDL_FLIP_NONE);
	}
	SDL_RenderPresent(this->renderer);
}
#include "Vista.h"

Vista::Vista(EscenarioParseado* e){
	SDL_Init( SDL_INIT_EVERYTHING );
	this->window = SDL_CreateWindow("Worms!", 50, 50, e->anchoPx, e->altoPx,  SDL_WINDOW_SHOWN );
	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	this->listaDibujables = new list<Dibujable*>;
	this->crearDibujable(0,0,e->anchoPx,e->nivelAgua * e->altoPx / e->altoU, texturaCielo);
	this->crearDibujable(0,e->nivelAgua * e->altoPx / e->altoU, e->anchoPx,(e->altoU - e->nivelAgua) * e->altoPx / e->altoU,texturaAgua);
}

Vista::~Vista() {
	delete this->window;
	delete this->renderer;
	delete this->listaDibujables;
}
SDL_Renderer* Vista::getRenderer() {
	return this->renderer;
}

Dibujable* Vista::crearDibujable(int x , int y ,int ancho,int alto, string pathImagen) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = ancho;
	rect.h = alto;
	
	Dibujable* dib = new Dibujable(this->renderer, rect, pathImagen);
	this->agregarDibujable(dib);
	return dib;
}

FiguraDibujable* Vista::crearFiguraDibujable(int x , int y ,int ancho,int alto, string pathImagen) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = ancho;
	rect.h = alto;
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

void AgregarCieloAguaTierra(string pathCielo, string pathAgua, string pathTierra) {


}
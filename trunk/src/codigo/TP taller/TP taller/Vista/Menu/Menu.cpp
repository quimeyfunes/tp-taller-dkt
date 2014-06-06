#include "Menu.h"


Menu::Menu(SDL_Window* window,SDL_Renderer* renderer){
	this->window = window;
	this->renderer = renderer;
	this->listaDibujables = new list<DibujableTextura*>;
	SDL_GetWindowSize(window,&this->ancho,&this->alto);
	DibujableTextura* dib;
	SDL_Rect rect;	
	rect.x = this->ancho / 2;
	rect.y = this->alto * 1 / 3;
	rect.w = 100;
	rect.h = 50;
	dib = new CartelDibujable(this->renderer,rect,"Servidor",50,255,0,0);
	this->listaDibujables->push_back(dib);
}


void Menu::dibujar(){
	SDL_Event* e = NULL;
	SDL_PollEvent(e);	
	SDL_RenderClear(this->renderer);
	list<DibujableTextura*>::iterator it = this->listaDibujables->begin();
	
	while (it != this->listaDibujables->end())
	{
		(*it)->dibujar(this->renderer, 0, 0, 1, this->ancho, this->alto);
		++it;
		}
	SDL_RenderPresent(this->renderer);
}


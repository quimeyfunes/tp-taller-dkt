#include "BotonServidor.h"


BotonServidor::BotonServidor(SDL_Renderer* renderer, SDL_Rect rect, string nombre, int tamanioTexto, int r, int g, int b) :
Boton(renderer,rect,nombre,tamanioTexto,r,g,b)
{

}

BotonServidor::~BotonServidor(){

}


int BotonServidor::click(int x,int y){
	if (x > this->rect.x && x < (this->rect.x + this->rect.w) && y > this->rect.y && y < (this->rect.y + this->rect.h)) {
		return 2;
	}
	return 0;
}
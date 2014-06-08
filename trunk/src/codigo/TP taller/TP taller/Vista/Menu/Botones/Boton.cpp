#include "Boton.h"

Boton::Boton() {

}

Boton::Boton(SDL_Renderer* renderer, SDL_Rect rect, string nombre, int tamanioTexto, int r, int g, int b, bool centrado) :
CartelDibujable(renderer,rect,nombre,tamanioTexto,r,g,b,centrado)
{
	this->activo = false;
}


Boton::~Boton(){

}

int Boton::click(int x,int y){
	if (x > this->rect.x && x < (this->rect.x + this->rect.w) && y > this->rect.y && y < (this->rect.y + this->rect.h)) {
		return 1;
	}
	return 0;
}

void Boton::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx) {
	SDL_RenderCopy(renderer,this->imagen,NULL,&this->rect);
}

void Boton::setActivo(bool activo){
	this->activo = activo;
}

bool Boton::getActivo(){
	return this->activo;
}
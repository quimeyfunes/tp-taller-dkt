#include "Boton.h"

Boton::Boton() {

}

Boton::Boton(SDL_Renderer* renderer, SDL_Rect rect, string nombre, int tamanioTexto, int r, int g, int b) :
CartelDibujable(renderer,rect,nombre,tamanioTexto,r,g,b)
{

}

Boton::~Boton(){

}

int Boton::click(int x,int y){
	return 0;
}
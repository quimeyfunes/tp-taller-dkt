#include "FiguraDibujable.h"


FiguraDibujable::FiguraDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen) : Dibujable(renderer, rect, pathImagen)
{
};

FiguraDibujable::~FiguraDibujable() {
}

void FiguraDibujable::setColor(int* rgb){
	SDL_SetTextureColorMod(this->getImagen(),rgb[0],rgb[1],rgb[2]);
}
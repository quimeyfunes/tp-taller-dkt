#include "Dibujable.h"


Dibujable::Dibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen) {
	this->rect = rect;
	this->imagen = IMG_LoadTexture(renderer, pathImagen.c_str());
	this->anguloRotacion = 0;
};

Dibujable::~Dibujable() {
	delete this->imagen;
}

SDL_Texture* Dibujable::getImagen(){
	return this->imagen;
}

SDL_Rect Dibujable::getRect(){
	return this->rect;
}

int Dibujable::getAngulo() {
	return this->anguloRotacion;
}

void Dibujable::setAngulo(int angulo) {
	this->anguloRotacion = angulo;
}
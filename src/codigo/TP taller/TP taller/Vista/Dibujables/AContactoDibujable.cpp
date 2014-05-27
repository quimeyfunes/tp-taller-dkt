#include "AContactoDibujable.h"


AContactoDibujable::AContactoDibujable(void)
{
}

AContactoDibujable::AContactoDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF)
	: ArmaDibujable(renderer, rect, pathImagen, pathDEF)
{
	
}

void AContactoDibujable::actualizar(Observable* observable){

	Arma* fig = (Arma*)observable;
	if (fig->getExplotar()) {
		this->destruido = true;
	} else {
		this->rect.x = (fig->getPosicion().x * relacionPPU) - rect.w /2;
		this->rect.y = (fig->getPosicion().y * relacionPPU) - rect.h /2;
		b2Vec2 velocidad = fig->getBody()->GetLinearVelocity();
		this->anguloRotacion = atan2(velocidad.y,velocidad.x) * RADTODEG + 180;
	}
}

AContactoDibujable::~AContactoDibujable(void)
{
}

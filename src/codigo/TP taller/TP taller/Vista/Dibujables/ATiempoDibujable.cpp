#include "ATiempoDibujable.h"


ATiempoDibujable::ATiempoDibujable(void)
{
}

ATiempoDibujable::ATiempoDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF)
	: ArmaDibujable(renderer, rect, pathImagen, pathDEF)
{
	
}

void ATiempoDibujable::actualizar(Observable* observable){

	Arma* fig = (Arma*)observable;
	if (fig->getExplotar()) {
		this->destruido = true;
	} else {
		this->rect.x = (fig->getPosicion().x * relacionPPU) - rect.w /2;
		this->rect.y = (fig->getPosicion().y * relacionPPU) - rect.h /2;
		this->anguloRotacion = fig->getBody()->GetAngle() * RADTODEG;
	}
}

ATiempoDibujable::~ATiempoDibujable(void)
{
}

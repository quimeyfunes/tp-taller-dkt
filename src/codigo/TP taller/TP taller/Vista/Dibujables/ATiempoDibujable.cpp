#include "ATiempoDibujable.h"


ATiempoDibujable::ATiempoDibujable(void)
{
}

ATiempoDibujable::ATiempoDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF)
	: ArmaDibujable(renderer, rect, pathImagen, pathDEF)
{
	SDL_Rect rectReloj;
	rectReloj.w = 10;
	rectReloj.h = 10;
	this->reloj = new RelojArmaSprite(renderer,rectReloj,spriteRelojRojo);
}

void ATiempoDibujable::actualizar(Observable* observable){
	ExplosivaPorTiempo* fig = (ExplosivaPorTiempo*)observable;
	fig->actualizar();
	if (fig->getExplotar()) {
		this->destruido = true;
	} else {
		this->rect.x = (fig->getPosicion().x * relacionPPU) - rect.w /2;
		this->rect.y = (fig->getPosicion().y * relacionPPU) - rect.h /2;
		this->anguloRotacion = fig->getBody()->GetAngle() * RADTODEG;
		SDL_Rect rectReloj = this->reloj->getRect();
		rectReloj.x = this->rect.x;
		rectReloj.y = this->rect.y - 5;
		this->reloj->setRect(rectReloj);
		this->reloj->setTiempoRestante(fig->getTiempoRestante());
	}
}

void ATiempoDibujable::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx){
	ArmaDibujable::dibujar(renderer,corrimientoX,corrimientoY,escalaZoom,anchoPx,altoPx);
	this->reloj->dibujar(renderer,corrimientoX,corrimientoY,escalaZoom,anchoPx,altoPx);
}

ATiempoDibujable::~ATiempoDibujable(void)
{
	delete this->reloj;
}

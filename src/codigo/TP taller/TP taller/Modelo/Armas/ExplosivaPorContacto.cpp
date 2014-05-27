#include "ExplosivaPorContacto.h"


ExplosivaPorContacto::ExplosivaPorContacto(void)
{
}


ExplosivaPorContacto::ExplosivaPorContacto(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa, float radio)
	: Arma(x , y , rotacion, world, estatico, false, radio)
{
}

void ExplosivaPorContacto::BeginContact() {
	this->explotar(true);
}

ExplosivaPorContacto::~ExplosivaPorContacto(void)
{
}

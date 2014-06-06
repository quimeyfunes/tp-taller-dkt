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

void ExplosivaPorContacto::actualizar(int anchoU, int altoU) {
	b2Vec2 pos = this->getPosicion();
	if (pos.x < -0.5 * anchoU || pos.x > 1.5 * anchoU || pos.y > 1.5 * altoU){
		this->explota = true;
	}
}

ExplosivaPorContacto::~ExplosivaPorContacto(void)
{
}

#include "ExplosivaPorTiempo.h"


ExplosivaPorTiempo::ExplosivaPorTiempo(void)
{
}

ExplosivaPorTiempo::ExplosivaPorTiempo(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float masa, int tiempoExplosion)	: Arma(x,y, rotacion, world, estatico, false, radioExplosion)
{
	this->tiempoExplosion = tiempoExplosion;
	this->tiempoInicial = time(NULL);

}

bool ExplosivaPorTiempo::getExplotar(){
	if (time(NULL) - this->tiempoInicial >= this->tiempoExplosion){
		this->explotar(true);
	}else this->explotar(false);

	return this->explota;
}

ExplosivaPorTiempo::~ExplosivaPorTiempo(void)
{
}

void ExplosivaPorTiempo::BeginContact(){
cout<<"hola"<<endl;
}

void ExplosivaPorTiempo::EndContact(){}
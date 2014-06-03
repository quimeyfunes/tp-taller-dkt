#include "Aleluya.h"


Aleluya::Aleluya(void)
{
}

Aleluya::Aleluya(float x, float y, short int rotacion, b2World* world, bool estatico, float radioExplosion, float radioArma, float masa)
	: ExplosivaPorTiempo(x,y,rotacion, world, estatico, radioExplosion, masa, tiempoExplosionAleluya)
{
	aleluya=false;
	this->armaTipo = ALELUYA;
	b2CircleShape circleShape;
	circleShape.m_radius = radioArma;
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape; 
	float areaCirculo = b2_pi * radioArma * radioArma;
	fixtureDef.density = masa/areaCirculo;
	fixtureDef.restitution = restitucion;
	fixtureDef.friction = friccion;
	fixtureDef.userData = this;
	this->getBody()->CreateFixture(&fixtureDef);
}

Aleluya::~Aleluya(void)
{
}


void Aleluya::disparar(bool sentido, float potencia, float angulo){

	float vX = potencia*coseno[(int)angulo];//os(angulo*DEGTORAD);
	float vY = -potencia*seno[(int)angulo];//in(angulo*DEGTORAD);

	if(sentido) vX *= -1;

	this->getBody()->SetLinearVelocity(b2Vec2(vX, vY));
	this->getBody()->SetAngularVelocity(2);
	Reproductor::getReproductor()->reproducirSonido(SOLTARGRANADA);
}

void Aleluya::BeginContact(){
	Reproductor::getReproductor()->reproducirSonido(IMPACTOALELUYA);
}

bool Aleluya::getExplotar(){
	this->explota = false;

	if(!aleluya){
		if(time(NULL) - this->tiempoInicial == this->tiempoExplosion){
			Reproductor::getReproductor()->reproducirSonido(SONIDOALELUYA);
			aleluya=true;
		}
	}else{
		if(Reproductor::getReproductor()->estaReproduciendo(SONIDOALELUYA)) this->explota=false;
		else
			this->explota=true;
	}
	return this->explota;
}
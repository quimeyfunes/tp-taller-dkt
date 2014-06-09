#include "MotorParticulas.h"
#include "../../constantesArmas.h"


MotorParticulas::MotorParticulas(SDL_Renderer* renderer){

	this->renderer = renderer;
	//levanto las posibles texturas de particula
	this->texturas.push_back(rutaCirculo);
	this->texturas.push_back(rutaEstrella);
	this->texturas.push_back(rutaDiamante);

	this->particulas = new vector<Particula*>;
}


MotorParticulas::~MotorParticulas()
{
}

void MotorParticulas::generarExplosion(int x, int y){

	int particulasPorExplosion = 30;

	for(int i=0; i< particulasPorExplosion; i++){
		this->particulas->push_back(generarParticula(x, y));
	}

}

Particula* MotorParticulas::generarParticula(int x, int y){

	string textura = this->texturas.at(rand()%this->texturas.size());

    int vX = rand()%20 -10;
    int vY = rand()%20 -10;
    float angle = 0;
    float angularVelocity = 0.5f + rand()%2;

    int ttl = 6 + rand()%20;
	this->pos.x=x;
	this->pos.y=y;
	this->pos.h=2 + rand()%5;
	this->pos.w=2 + rand()%5;
	
	return new Particula(renderer, this->pos, vX, vY, angle, angularVelocity, ttl, textura);
}

void MotorParticulas::actualizar(){
 
	for (int particle = 0; particle < this->particulas->size(); particle++)
    {
		this->particulas->at(particle)->actualizar();
		if (this->particulas->at(particle)->tiempoDeVida <= 0)
        {
			this->particulas->erase(this->particulas->begin() + particle);
            particle--;
        }
    }
}

void MotorParticulas::dibujar(SDL_Renderer* renderer, int corrimientoX, int corrimientoY, int escalaZoom, int anchoPx, int altoPx){
	for (int index = 0; index < particulas->size(); index++){
		particulas->at(index)->dibujar(renderer, corrimientoX, corrimientoY, escalaZoom, anchoPx, altoPx);
    }
}

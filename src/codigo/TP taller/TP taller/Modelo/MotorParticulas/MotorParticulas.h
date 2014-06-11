#pragma once
#include "../../Vista/Dibujables/Particula.h"
#include <stdlib.h>

class MotorParticulas
{

	typedef struct{
		int x;
		int y;
		int numPartic;
	}genpart;

private:
	SDL_Rect pos;
	SDL_Renderer* renderer;
	vector<Particula*>* particulas;
	vector<string> texturas;
	Particula* generarParticula(int x, int y);
	genpart bufferParticulas;

public:
	MotorParticulas(SDL_Renderer* renderer);
	~MotorParticulas();
	
	void generarExplosion(int x, int y);
	void actualizar();
	void dibujar(SDL_Renderer* renderer, int corrimientoX, int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
};


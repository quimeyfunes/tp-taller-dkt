#pragma once
#include "DibujableTextura.h"

class Particula: DibujableTextura{
public:
	Particula(SDL_Renderer* renderer, SDL_Rect pos, int vX, int vY, float angulo, float vAngular, int TTL, string pathImagen);
	~Particula(void);
	SDL_Renderer* ren;
	void actualizar();
	void dibujar(SDL_Renderer* renderer, int corrimientoX, int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
	int tiempoDeVida;
private:

	SDL_Rect posicion;
	int vX;
	int vY;
	float angulo;
	float velocAngular;
	
};


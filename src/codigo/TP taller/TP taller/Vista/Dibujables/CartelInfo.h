#include "CartelDibujable.h"

#pragma once
class CartelInfo: public CartelDibujable
{
public:
	CartelInfo(SDL_Renderer* renderer, SDL_Rect rect);
	~CartelInfo();
	void setInfo(string info);
	void setColor(int R, int G, int B, int A);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);


private:

	SDL_Renderer* renderer;

	int tiempoDibujando;
	bool dibujado;
	int tiempoMaximo;

};


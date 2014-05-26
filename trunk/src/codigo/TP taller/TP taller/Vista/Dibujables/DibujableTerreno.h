#pragma once
#include "dibujableTextura.h"
class DibujableTerreno :
	public DibujableTextura
{
public:
	DibujableTerreno(void);
	DibujableTerreno(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF);
	~DibujableTerreno(void);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);

};


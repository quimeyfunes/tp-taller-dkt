#ifndef __DIBUJABLE_TEXTURA_H__
#define __DIBUJABLE_TEXTURA_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Dibujable.h"
#include "../../Parser/yaml/ParserYaml.h"
#include <string>

using namespace std;

class DibujableTextura: public Dibujable
{
	SDL_Rect rect;
	SDL_Texture* imagen;
	float anguloRotacion;
public:
	DibujableTextura();
	DibujableTextura(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF);
	~DibujableTextura();
	SDL_Rect getRect();
	void setRect(SDL_Rect rect);
	SDL_Texture* getImagen();
	int getAngulo();
	void setAngulo(int angulo);
	void setColor(int* rgb, int a);
	void dibujar(SDL_Renderer* renderer, int corrimiento, float escalaZoom, int posZoomX, int posZoomY);
	DibujableSerializado getDibujableSerializado(int& tamano);
	SDL_Rect realizarZoom(SDL_Rect rect, int posX, int PosY, float escalaZoom);
};

#endif
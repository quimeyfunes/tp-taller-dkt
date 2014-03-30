#ifndef __DIBUJABLE_H__
#define __DIBUJABLE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../Parser/yaml/ParserYaml.h"
#include <string>

using namespace std;

class Dibujable
{
	SDL_Rect rect;
	SDL_Texture* imagen;
	float anguloRotacion;
public:
	Dibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen);
	~Dibujable();
	SDL_Rect getRect();
	void setRect(SDL_Rect rect);
	SDL_Texture* getImagen();
	int getAngulo();
	void setAngulo(int angulo);
	void setColor(int* rgb);
};

#endif
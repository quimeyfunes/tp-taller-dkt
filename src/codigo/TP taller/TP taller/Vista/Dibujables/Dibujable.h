#ifndef __DIBUJABLE_H__
#define __DIBUJABLE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL2_gfxPrimitives.h"
#include "../../Parser/yaml/ParserYaml.h"
#include <string>

using namespace std;

class Dibujable
{
public:
	Dibujable();
	~Dibujable();
	virtual void setColor(int* rgb, int a);
	virtual void dibujar(SDL_Renderer* renderer, int corrimiento);
};

#endif
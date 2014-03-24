#ifndef __FIGURA_DIBUJABLE_H__
#define __FIGURA_DIBUJABLE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Dibujable.h"

using namespace std;

class FiguraDibujable : public Dibujable
{

public:
	FiguraDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen);
	~FiguraDibujable();
	void setColor(int* rgb);
};

#endif
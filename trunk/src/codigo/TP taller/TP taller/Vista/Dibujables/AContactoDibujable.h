#ifndef __ACONTACTO_DIBUJABLE_H__
#define __ACONTACTO_DIBUJABLE_H__

#include "ArmaDibujable.h"
class AContactoDibujable: public ArmaDibujable
{
public:
	AContactoDibujable(void);
	AContactoDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF);
	void actualizar(Observable* observable);
	~AContactoDibujable(void);
};

#endif
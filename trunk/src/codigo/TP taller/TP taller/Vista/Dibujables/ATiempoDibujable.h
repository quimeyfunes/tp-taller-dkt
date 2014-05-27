#ifndef __ATIEMPO_DIBUJABLE_H__
#define __ATIEMPO_DIBUJABLE_H__

#include "ArmaDibujable.h"
class ATiempoDibujable: public ArmaDibujable
{
public:
	ATiempoDibujable(void);
	ATiempoDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF);
	void actualizar(Observable* observable);
	~ATiempoDibujable(void);
};

#endif
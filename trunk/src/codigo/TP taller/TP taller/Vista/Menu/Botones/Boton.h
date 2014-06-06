#ifndef BOTON_H
#define BOTON_H

#include "../../Dibujables/CartelDibujable.h"

class Boton : public CartelDibujable{
	
private:

public:

	Boton();
	Boton(SDL_Renderer* renderer, SDL_Rect rect, string nombre, int tamanioTexto, int r, int g, int b);
	~Boton();

	virtual int click(int x,int y);

};

#endif
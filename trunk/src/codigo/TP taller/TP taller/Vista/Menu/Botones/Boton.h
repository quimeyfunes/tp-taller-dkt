#ifndef BOTON_H
#define BOTON_H

#include "../../Dibujables/CartelDibujable.h"

class Boton : public CartelDibujable{
	
public:

	Boton();
	Boton(SDL_Renderer* renderer, SDL_Rect rect, string nombre, int tamanioTexto, int r, int g, int b);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);	
	~Boton();

	virtual int click(int x,int y);

};

#endif
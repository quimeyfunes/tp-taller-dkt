#ifndef BOTON_H
#define BOTON_H

#include "../../Dibujables/CartelDibujable.h"

class Boton : public CartelDibujable{
protected:
	bool activo;

public:

	Boton();
	Boton(SDL_Renderer* renderer, SDL_Rect rect, string nombre, int tamanioTexto, int r, int g, int b, bool centrado);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);	
	void setActivo(bool activo);
	bool getActivo();	
	~Boton();

	virtual int click(int x,int y);

};

#endif
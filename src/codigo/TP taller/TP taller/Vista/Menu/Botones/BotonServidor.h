#ifndef BOTON_SERVIDOR_H
#define BOTON_SERVIDOR_H

#include "Boton.h"

class BotonServidor: public Boton {
	
private:

public:

	BotonServidor();
	BotonServidor(SDL_Renderer* renderer, SDL_Rect rect, string nombre, int tamanioTexto, int r, int g, int b);
	~BotonServidor();

	int click(int x,int y);

};

#endif
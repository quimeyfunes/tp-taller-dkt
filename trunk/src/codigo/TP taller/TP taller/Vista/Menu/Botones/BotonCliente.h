#ifndef BOTON_CLIENTE_H
#define BOTON_CLIENTE_H

#include "Boton.h"

class BotonCliente: public Boton {
	
private:

public:

	BotonCliente();
	BotonCliente(SDL_Renderer* renderer, SDL_Rect rect, string nombre, int tamanioTexto, int r, int g, int b);
	~BotonCliente();

	int click(int x,int y);
};

#endif
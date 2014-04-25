#ifndef __GUSANO_DIBUJABLE_H__
#define __GUSANO_DIBUJABLE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Dibujable.h"
#include "DibujableGFX.h"
#include "DibujableTextura.h"
#include "../../constantes.h"
#include "../../Modelo/Escenario/Escenario.h"
#include "../../Parser/yaml/ParserYaml.h"
#include "../../Observador/Observador.h"
#include "../../Modelo/Figuras/Figura.h"
#include <string>

class GusanoDibujable: public DibujableTextura, public Observador{
private:
	SDL_Rect rect;
	SDL_Texture* imagen;
	//SDL_Surface* superficie;
	float anguloRotacion;

public:
	GusanoDibujable();
	GusanoDibujable(SDL_Renderer* renderer, SDL_Rect rect);
	~GusanoDibujable();
	void actualizar(Observable* observable);
	void dibujar(SDL_Renderer* renderer, int corrimiento, float escalaZoom, int posZoomX, int posZoomY);
};

#endif
#ifndef __CARTEL_DIBUJABLE_H__
#define __CARTEL_DIBUJABLE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "Dibujable.h"
#include "DibujableTextura.h"
#include "../../constantes.h"
#include "../../Modelo/Escenario/Escenario.h"
#include "../../Parser/yaml/ParserYaml.h"
#include "../../Observador/Observador.h"
#include "../../Modelo/Figuras/Figura.h"
#include <string>


class CartelDibujable: public DibujableTextura{
public:
	CartelDibujable(void);
	CartelDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
	~CartelDibujable(void);
};

#endif
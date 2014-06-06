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
#include "SDL/SDL_ttf.h"
#include <string>
#include "../../constantes.h"

class CartelDibujable: public DibujableTextura{
protected:
	TTF_Font* font;
	SDL_Surface* surf;
	SDL_Color textColor;

public:
	CartelDibujable(void);
	CartelDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF, string nombre);
	CartelDibujable(SDL_Renderer* renderer, SDL_Rect rect, string nombre, int tamanioTexto, int r, int g, int b);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
	~CartelDibujable(void);
	string texto;
};

#endif
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

public:
	GusanoDibujable();
	GusanoDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF);
	~GusanoDibujable();
	void actualizar(Observable* observable);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
	virtual string serializar();
	virtual void deserealizar(string aDeserealizar);
};

#endif
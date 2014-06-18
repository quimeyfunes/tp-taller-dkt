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
#include "CartelDibujable.h"

class GusanoDibujable: public DibujableTextura, public Observador{
private:
	bool cambiarImgDer;
	bool cambiarImgIzq;
	int contIzq;
	int contDer;
	int contFrent;
	CartelDibujable* cartel;
	bool mostrarCartel;
public:
	GusanoDibujable();
	GusanoDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF);
	~GusanoDibujable();
	void actualizar(Observable* observable);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
	CartelDibujable* getCartel();
	virtual string serializar();
	virtual int deserealizar(string aDeserealizar);
	void setCambiarImgDer(bool cambio);
	bool hayCambioImgDer();
	void setCambiarImgIzq(bool cambio);
	bool hayCambioImgIzq();
};

#endif
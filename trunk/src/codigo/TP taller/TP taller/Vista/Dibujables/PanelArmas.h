#ifndef __PANELARMAS_H__
#define __PANELARMAS_H__

#include "DibujableTextura.h"
#include <string>
#include "../../constantes.h"

class PanelArmas: public DibujableTextura
{
public:
	PanelArmas(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF);
	~PanelArmas();
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
	void alternarVisibilidad();

private:
	SDL_Renderer* renderer;
	bool visible;
};

#endif
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
	bool visible;
	void agregarArma(SDL_Renderer* renderer, string nombreArmaSeleccionada, string nombreArmaDeseleccionada);
	void seleccionarArma(int numeroArma);
private:
	SDL_Renderer* renderer;
	vector<DibujableTextura*> armasTexturas;
	vector<string> nombresArmasSeleccionada;
	vector<string> nombresArmasDeseleccionada;
	int armaSeleccionada;
};

#endif
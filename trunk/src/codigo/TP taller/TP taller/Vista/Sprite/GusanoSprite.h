#ifndef __GUSANO_SPRITE_H__
#define __GUSANO_SPRITE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../Dibujables/DibujableTextura.h"
#include "../../constantes.h"
#include "../../Modelo/Escenario/Escenario.h"
#include "../../Parser/yaml/ParserYaml.h"
#include "../../Observador/Observador.h"
#include "../../Modelo/Figuras/Figura.h"
#include <string>

class GusanoSprite: public DibujableTextura, public Observador{
private:
	SDL_Rect* recCuadro;
	int frame;
	int numCuadros;
	int velocidadRefresco;
	int contador;
	bool cambiarImgDer;
	bool cambiarImgIzq;
	int contIzq;
	int contDer;
	int contFrent;

public:
	GusanoSprite(void);
	GusanoSprite(SDL_Renderer* renderer, SDL_Rect recFrame, string path, int col, int fil, int anchoTex, int altoTex);
	void actualizar(Observable* observable);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
	virtual string serializar();
	virtual void deserealizar(string aDeserealizar);
	void setCambiarImgDer(bool cambio);
	bool hayCambioImgDer();
	void setCambiarImgIzq(bool cambio);
	void setNumCuadros(int numCuadros);
	void actualizarFrame();
	int getNumCuadros();
	bool hayCambioImgIzq();
	~GusanoSprite(void);
};

#endif
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
#include "../Dibujables/CartelDibujable.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

typedef enum{
	IZQ,
	DER,
	MUERTO,
}ESTADO;

class GusanoSprite: public DibujableTextura, public Observador{
private:
	SDL_Rect* recCuadro;
	SDL_Rect* rectApuntando;
	int frame;
	bool cambiarImgDer;
	bool cambiarImgIzq;
	CartelDibujable* cartel;
	string nombre;
	int maximosCLientes;
public:
	GusanoSprite(void);
	GusanoSprite(SDL_Renderer* renderer, SDL_Rect recFrame, string path, int col, int fil, int anchoTex, int altoTex, string nombre,int maximosClientes);
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
	int getFrame();
	void setFrame(int frame);
	bool hayCambioImgIzq();
	~GusanoSprite(void);
	CartelDibujable* getCartel();
	string getNombre();
	int contador;
	int contIzq;
	int contDer;
	int contFrent;
	int contMuerte;
	ESTADO estado;
	vector<bool> mostrarCartel;
	int velocidadRefresco;
	int numCuadros;
	int cliente;
	int congelado;
};

#endif
#ifndef __GUSANO_SPRITE_H__
#define __GUSANO_SPRITE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "../Dibujables/DibujableTextura.h"
#include "../../constantes.h"
#include "../../constantesArmas.h"
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
	SDL_Texture* crosshair;
	SDL_Point* posFigura;
	int frameDisparo;
	int frame;
	bool cambiarImgDer;
	bool cambiarImgIzq;
	bool muertePorDisparo;
	CartelDibujable* cartel;
	DibujableTextura* vida;
	DibujableTextura* flecha;
	bool activo;
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
	void actualizarFrameDisparo(int frame);
	void actualizarFrameCrosshair(int potencia);
	bool actualizarFrameTnt();
	bool actualizarFrameSuicida();
	void actualizarFrameGrave();
	int getNumCuadros();
	int getFrame();
	void setFrame(int frame);
	bool hayCambioImgIzq();
	~GusanoSprite(void);
	CartelDibujable* getCartel();
	DibujableTextura* getVida();
	string getNombre();
	void copiarGusano(GusanoSprite* gusano2, bool nuevo);
	int contador;
	int contIzq;
	int contDer;
	int contFrent;
	int contMuerte;
	int contArma;
	int contMuerteVida;
	tipoArma armaTipo;
	ESTADO estado;
	vector<bool> mostrarCartel;
	float velocidadRefresco;
	int numCuadros;
	int cliente;
	int congelado;
	bool mostrarCrosshair;
	int frameCrosshair;
	int anguloDisparo;
	SDL_Rect* enUso;
	SDL_Rect* recPotencia;
	SDL_Rect* rectApuntando;
	SDL_Rect* recCuadro;
	SDL_Rect* rectTnt;
	SDL_Rect* rectGrave;
	SDL_Rect* rectSuicida;
	bool terminoIteracion;
	int vidaValor;
	float fraccionVidaValor;
	bool huboCambioArma;
	bool ahogado;
};

#endif
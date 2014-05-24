#ifndef __VISTA_H__
#define __VISTA_H__
#include "Dibujables/DibujableTextura.h"
#include "Dibujables/CirculoDibujable.h"
#include "Dibujables/PoligonoDibujable.h"
#include "Dibujables\RectanguloDibujable.h"
#include "Dibujables\CartelDibujable.h"
#include "Dibujables\GusanoDibujable.h"
#include "Dibujables/CartelInfo.h"
#include "Dibujables/PanelArmas.h"
#include "Sprite/Sprite.h"
#include "Sprite\GusanoSprite.h"
#include "Sprite/ScrollingSprite.h"
#include "../Parser/yaml/ParserYaml.h"
#include "SDL/SDL_ttf.h"
#include <list>
#include "Dibujables\ArmaDibujable.h"

using namespace std;

typedef enum{
	PAUSAR,
	JUGAR,
	SALIR,
	IZQUIERDA,
	ARRIBA,
	ABAJO,
	SOLTARABAJO,
	DERECHA,
	SOLTARARRIBA,
	SOLTARDERECHA,
	SOLTARIZQUIERDA,
	CLICK,
	CLICKDERECHO,
	ESPACIO,
	SOLTARESPACIO,
}ACCION_REALIZADA;

class Vista
{

private:

	
	
	list<Dibujable*>* listaDibujables;
	int anchoPx;
	int altoPx;
	int anchoPxTot;
	int altoPxTot;
	float corrimientoX;
	float corrimientoY;
	ACCION_REALIZADA accion;
	float escalaZoom;

public:
	SDL_Window* window;
	SDL_Renderer* renderer;
	Vista(EscenarioParseado* e);
	Vista();
	~Vista();
	SDL_Renderer* getRenderer();
	DibujableTextura* crearDibujableTextura(int x , int y ,int ancho,int alto, string pathImagen, string imagenDEF);
	Sprite* crearSprite(int x, int y, int anchoFrame, int altoFrame, string path, int col, int fil, int anchoTex, int altoTex);
	void crearScrollingSprite(int x, int y, int ancho, int alto, string path);
	CirculoDibujable* crearCirculoDibujable(int x , int y ,int radioHorizontal,int radioVertical);
	PoligonoDibujable* crearPoligonoDibujable(short int n, float escalaAncho, float escalaAlto);
	RectanguloDibujable* crearRectanguloDibujable(float ancho, float alto);
	GusanoDibujable* crearGusanoDibujable(int x, int y, int ancho, int alto, string pathImagen, string imagenDef);
	GusanoSprite* crearGusanoSprite(int x, int y, int anchoFrame, int altoFrame, string path, int col, int fil, int anchoTex, int altoTex,string nombre,int maximosClientes);
	CartelInfo* crearCartelInfo(int x, int y, int ancho, int alto);
	ArmaDibujable* crearArmaDibujable(int x, int y, int ancho, int alto, string pathImagen, string imagenDef);
	PanelArmas* Vista::crearPanelArmas(int x , int y ,int ancho,int alto);
	void agregarDibujable(Dibujable* dibujable);
	list<Dibujable*>* getListaDibujables();
	void setListaDibujables(list<Dibujable*>* dibujables);
	void Dibujar();
	int getAnchoPx();
	int getAltoPx();
	bool leerEvento(SDL_Event* evento);
	ACCION_REALIZADA getAccion();
	float getCorrimientoX();
	float getCorrimientoY();
	float getZoom();
	void setZoom(float escala);
	void scroll(int x ,int y );
	void zoom(SDL_Event* evento,int x, int y);
	void validarCorrimiento();

	void destruir(float x,float y,int radio, LectorTerreno* lector);
};

#endif
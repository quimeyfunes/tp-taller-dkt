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
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
class RelojSprite: public DibujableTextura, public Observador
{

private:

	SDL_Rect* recCuadro;
	int frame1; //primer digito
	int frame2; //segundo digito
	int numCuadros;
	int contadorDePitidos;
	int tiempoActual;
	string imagenReloj;
	string imagenRelojRojo;
	
public:
	RelojSprite(void);
	void setTiempoActual(int tiempo);
	RelojSprite(SDL_Renderer* renderer, SDL_Rect recFrame, string path, int col, int fil, int anchoTex, int altoTex);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);

	~RelojSprite(void);
};


#include "DibujableTextura.h"



class RelojDibujable: public DibujableTextura
{
public:
	RelojDibujable(void);
	~RelojDibujable(void);
	RelojDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
};


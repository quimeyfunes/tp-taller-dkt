#ifndef __ATIEMPO_DIBUJABLE_H__
#define __ATIEMPO_DIBUJABLE_H__

#include "ArmaDibujable.h"
#include "../Sprite/RelojArmaSprite.h"
#include "../../Modelo/Armas/ExplosivaPorTiempo.h"

class ATiempoDibujable: public ArmaDibujable
{
private:
	
public:
	ATiempoDibujable(void);
	ATiempoDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF);
	void actualizar(Observable* observable);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx);
	~ATiempoDibujable(void);
	virtual string serializar();
	virtual int deserealizar(string aDeserealizar);
	RelojArmaSprite* reloj;
};

#endif
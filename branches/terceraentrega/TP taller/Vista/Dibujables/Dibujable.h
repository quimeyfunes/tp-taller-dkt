#ifndef __DIBUJABLE_H__
#define __DIBUJABLE_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL2_gfxPrimitives.h"
#include "Box2D/Box2D.h"
#include "../../Parser/yaml/ParserYaml.h"
#include <string>
#include "../../Modelo/Red/Serializable.h"
#include "../../Parser/ParserDeHexARgb.h"

using namespace std;

class Dibujable: public Serializable
{
public:
	Dibujable();
	virtual ~Dibujable();
	virtual void setColor(int* rgb);
	virtual void dibujar(SDL_Renderer* renderer, int corrimientoX, int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
	virtual string serializar();
	virtual void deserealizar(string aDeserealizar);
};

#endif
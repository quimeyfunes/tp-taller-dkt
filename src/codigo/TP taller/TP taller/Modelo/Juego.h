#ifndef __JUEGO_H__
#define __JUEGO_H__

#include "SDL/SDL.h"
#include "Box2D/Box2D.h"
#include "SDL/SDL_image.h"
#include "yaml-cpp\yaml.h"
#include "../Vista\Vista.h"
#include "Escenario.h"
#include "../Parser/ParserDeHexARgb.h"
#include "Terreno.h"
#include <iostream>
#include <vector>

using namespace std;

class Juego
{
private:
    bool enPausa;
public:
	Juego();
	void comenzar();
	void pausar();
	bool getPausa();
};


#endif
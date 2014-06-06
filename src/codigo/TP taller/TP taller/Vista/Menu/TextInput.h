#ifndef __TEXT_INPUT_H__
#define __TEXT_INPUT_H__

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include <string>
#include <iostream>
#include "../../constantes.h"
#include "../Dibujables/DibujableTextura.h"

using namespace std;

class TextInput: public DibujableTextura 
{
private:
	TTF_Font* font;
	SDL_Surface* surf;
	string texto;
	SDL_Color textoColor;
public:
	TextInput(void);
	TextInput(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF, string nombre);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx); 
	bool loadFromRenderedText(SDL_Renderer* renderer, string textureText, SDL_Color textColor );
	~TextInput(void);
};

#endif

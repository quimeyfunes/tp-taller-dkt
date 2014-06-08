#ifndef __TEXT_INPUT_H__
#define __TEXT_INPUT_H__

#include "../../../constantes.h"
#include "Boton.h"

using namespace std;

class TextInput: public Boton
{
private:
	bool activo;
	CartelDibujable* input;
public:
	TextInput(void);
	TextInput(SDL_Renderer* renderer, SDL_Rect rect, string nombre, int tamanioTexto, int r, int g, int b,bool centrado);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx); 
	bool loadFromRenderedText(SDL_Renderer* renderer, string textureText);
	string getTexto();
	~TextInput(void);
};

#endif

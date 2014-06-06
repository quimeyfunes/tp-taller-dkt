#ifndef __MENU_H__
#define __MENU_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "Botones\BotonCliente.h"
#include "Botones\BotonServidor.h"
#include "TextInput.h"


namespace nameMenu {
	
	typedef enum{
		SALIR = 1,
		SERVIDOR = 2,
		CLIENTE = 3,
	}ACCION_REALIZADA;
}


class Menu {

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	list<Boton*>* listaDibujables;
	SDL_Event* evento;
	int ancho;
	int alto;
	TextInput* textInput;
public:
	Menu(SDL_Window* window, SDL_Renderer* renderer);
	void escribir();
	Menu();
	~Menu();
	int leerEvento();
	int clickServidor();
	int clickCliente();
	void dibujar();
};

	
#endif
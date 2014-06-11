#ifndef __MENU_H__
#define __MENU_H__

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "Botones\textinput.h"


namespace nameMenu {
	
	typedef enum{
		SALIR = 1,
		SERVIDOR = 2,
		CLIENTE = 3,
		SI = 4,
		NO = 5,
	}ACCION_REALIZADA;
}


class Menu {

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	list<Boton*>* listaDibujables;
	DibujableTextura* fondo;
	SDL_Event* evento;
	bool menuPrincipal;
	bool menuCliente;
	int ancho;
	int alto;
	TextInput* textInputNombre;
	TextInput* textInputIP;
public:
	Menu();
	Menu(SDL_Window* window, SDL_Renderer* renderer);
	void escribir(SDL_Event* evento);
	
	int leerEvento();
	void dibujar();
	
	string getNombre();
	string getIP();
	void setIP(string ip);
	void agregarMensaje(string nombre, int tamanioTexto, int r, int g, int b);

	void menuJugarDeNuevo();

	~Menu();
};

	
#endif
#include "Dibujable.h"
#include <list>

using namespace std;

class Vista
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	list<Dibujable*>* listaDibujables;
	
public:

	Vista();
	~Vista();
	SDL_Renderer* getRenderer();
	void crearDibujable(SDL_Rect rect, string pathImagen);
	void agregarDibujable(Dibujable* dibujable);
	list<Dibujable*>* getListaDibujables();
	void Dibujar();
	
};
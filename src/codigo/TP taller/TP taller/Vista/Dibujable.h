#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>

using namespace std;

class Dibujable
{
	SDL_Rect rect;
	SDL_Texture* imagen;
	float anguloRotacion;
public:
	Dibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen);
	~Dibujable();
	SDL_Rect getRect();
	SDL_Texture* getImagen();
	int getAngulo();
	void setAngulo(int angulo);
};
#include "../Dibujables/DibujableTextura.h"

class ScrollingSprite: public DibujableTextura{

private:

	SDL_Rect rec;
	SDL_Texture* textura;
	int velocidadRefresco;
	int anchoEscenario;
	int contador;

public:
	ScrollingSprite(SDL_Renderer* renderer, SDL_Rect rect, string path);
	virtual void dibujar(SDL_Renderer* renderer, int corrimiento) override;
	~ScrollingSprite();
};


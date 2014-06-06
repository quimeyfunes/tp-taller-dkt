#include "../Dibujables/DibujableTextura.h"

class RelojArmaSprite: public DibujableTextura
{

private:
	SDL_Rect* recCuadro;
public:
	RelojArmaSprite(void);
	RelojArmaSprite(SDL_Renderer* renderer, SDL_Rect recFrame, string path);
	void dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx);
	void setTiempoRestante(int tiempo);
	~RelojArmaSprite(void);
	virtual string serializar();
	virtual void deserealizar(string aDeserealizar);
	int frame;
	int numCuadros;
	int tiempoActual;
};


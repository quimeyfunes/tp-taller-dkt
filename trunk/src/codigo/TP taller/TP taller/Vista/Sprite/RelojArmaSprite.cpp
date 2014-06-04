#include "RelojArmaSprite.h"


RelojArmaSprite::RelojArmaSprite(void)
{
}

RelojArmaSprite::RelojArmaSprite(SDL_Renderer* renderer, SDL_Rect recDestino, string path): DibujableTextura(){

	this->frame = 0;
	this->imagen = IMG_LoadTexture(renderer, spriteRelojRojo);

	this->numCuadros = 10;
	int tamanioCuadroX = 42;
	int tamanioCuadroY = 560 / 10;
	this->rect = recDestino;
	this->recCuadro = new SDL_Rect[numCuadros];

	for(int i = 0; i < this->numCuadros; i++){
		recCuadro[i].h = tamanioCuadroY;
		recCuadro[i].w = tamanioCuadroX;
		recCuadro[i].x = 0;
		recCuadro[i].y = i * tamanioCuadroY;
	}
}

RelojArmaSprite::~RelojArmaSprite(void)
{
	delete []this->recCuadro;
}

void RelojArmaSprite::setTiempoRestante(int tiempo){
	if (tiempo < 0) {
		this->tiempoActual = 0;
	} else {
		this->tiempoActual = tiempo;
	}
}

void RelojArmaSprite::dibujar(SDL_Renderer *renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx){
	
	SDL_Rect rect = realizarZoom(this->rect,corrimientoX,corrimientoY,escalaZoom);
	//if ((this->tiempoActual > 9) || (this->tiempoActual)) this->tiempoActual = 9;
	SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[this->tiempoActual], &rect);
}
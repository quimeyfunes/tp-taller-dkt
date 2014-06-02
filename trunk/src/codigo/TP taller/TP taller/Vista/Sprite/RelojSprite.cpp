#include "RelojSprite.h"


RelojSprite::RelojSprite(void)
{
}

RelojSprite::RelojSprite(SDL_Renderer* renderer, SDL_Rect recDestino, string path, int col, int fil, int anchoTex, int altoTex): DibujableTextura(){

	this->tiempoActual = 0;
	this->numCuadros = col*fil;
	this->velocidadRefresco = timeStepOlas;
	this->contador = 0;

	int tamanioCuadroX = anchoTex / col;
	int tamanioCuadroY = altoTex / fil;
	this->frame1 = 0;
	this->frame2 = 0;
	this->rect = recDestino;

	this->recCuadro = new SDL_Rect[numCuadros];
	for(int i=0; i< numCuadros; i++){
		recCuadro[i].h = tamanioCuadroY;
		recCuadro[i].w = tamanioCuadroX;
	}
	for(int i=0; i<fil; i++){
		for(int j=0; j<col; j++){

			recCuadro[j + i*col].x = j* tamanioCuadroX;
			recCuadro[j + i*col].y = i* tamanioCuadroY;
		}
	}
	
	this->imagen = IMG_LoadTexture(renderer, path.c_str());
	
}

RelojSprite::~RelojSprite(void)
{
	delete []this->recCuadro;
}

void RelojSprite::setTiempoActual(int tiempo){
	this->tiempoActual = tiempo;
}

void RelojSprite::dibujar(SDL_Renderer *renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx){

	SDL_Rect rect = this->rect;
	SDL_Rect rect2 = this->rect;
	rect2.x += rect.w;
	
	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
		rect = realizarZoom(rect, corrimientoX, corrimientoY, escalaZoom);
		rect2 = realizarZoom(rect, corrimientoX, corrimientoY, escalaZoom);
		SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[frame1], &rect);
		SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[frame2], &rect2);
		//SDL_RenderCopy(renderer, IMG_LoadTexture(renderer, path_imagen.c_str()), &this->recCuadro[frame2], &rect2);
	} else {
		rect.x -=corrimientoX;
		rect.y -=corrimientoY;
		rect2.x -=corrimientoX;
		rect2.y -=corrimientoY;
		SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[frame1], &rect);
		SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[frame2], &rect2);
	}

	//frame1 = tiempoActual-(tiempoActual%10);

	frame1= tiempoActual/10;
	frame2 = tiempoActual%10;
	
	
}
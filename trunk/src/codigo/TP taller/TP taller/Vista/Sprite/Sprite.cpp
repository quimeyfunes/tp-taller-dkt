#include "Sprite.h"

Sprite::Sprite(SDL_Renderer* renderer, SDL_Rect recDestino, string path, int col, int fil, int anchoTex, int altoTex): DibujableTextura(){

	this->numCuadros = col*fil;
	this->velocidadRefresco = timeStepOlas;
	this->contador = 0;

	int tamanioCuadroX = anchoTex / col;
	int tamanioCuadroY = altoTex / fil;
	this->frame = 0;
	this->recDest = recDestino;

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
	
	this->textura = IMG_LoadTexture(renderer, path.c_str());
	
}


Sprite::~Sprite(){
	delete []this->recCuadro;
}

void Sprite::dibujar(SDL_Renderer *renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx){

	SDL_Rect rect = this->recDest;

	

	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax) && (escalaZoom >= zoomMin)) {
		rect = realizarZoom(this->recDest, corrimientoX, corrimientoY, escalaZoom);
		SDL_RenderCopy(renderer, this->textura, &this->recCuadro[frame], &rect);
	} else {
		rect.x -=corrimientoX;
		rect.y -=corrimientoY;
		SDL_RenderCopy(renderer, this->textura, &this->recCuadro[frame], &rect);
	}

	contador++;
	if(contador >= this->velocidadRefresco){
		frame++;
		contador = 0;
	}
	if(frame >= this->numCuadros) frame = 0;
	
}
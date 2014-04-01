#include "Sprite.h"

Sprite::Sprite(SDL_Renderer* renderer, SDL_Rect recDestino, string path, int col, int fil, int anchoTex, int altoTex): DibujableTextura(renderer, recDestino, path){

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
}

void Sprite::dibujar(SDL_Renderer *renderer){

	SDL_RenderCopy(renderer, this->textura, &this->recCuadro[frame], &this->recDest);
	contador++;
	if(contador >= this->velocidadRefresco){
		frame++;
		contador = 0;
	}
	if(frame >= this->numCuadros) frame = 0;
	
}
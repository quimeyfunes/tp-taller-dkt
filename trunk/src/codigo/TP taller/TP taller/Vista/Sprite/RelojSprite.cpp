#include "RelojSprite.h"
#include "../../Modelo/Reproductor.h"


RelojSprite::RelojSprite(void)
{
}

RelojSprite::RelojSprite(SDL_Renderer* renderer, SDL_Rect recDestino, string path, int col, int fil, int anchoTex, int altoTex): DibujableTextura(){

	this->sono = false;
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
	this->imagenReloj = spriteReloj;
	this->imagenRelojRojo = spriteRelojRojo;
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
	
	frame1 = tiempoActual/10;
	frame2 = tiempoActual%10;
	
	if(frame1 < 1 && frame2 == tiempoCritico)	setImagen(renderer, imagenRelojRojo.c_str());
	if(tiempoActual == tiempoTurno)	setImagen(renderer, imagenReloj.c_str());

	SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[frame1], &rect);
	SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[frame2], &rect2);


		/*if(frame2 == 3 && !sono){
			Reproductor::getReproductor()->reproducirSonido(TIMERTICK);
			sono=true;
		}
		if(frame2 == 2 && sono){
			Reproductor::getReproductor()->reproducirSonido(TIMERTICK);
			sono=false;
		}
		if(frame2 == 1 && !sono){
			Reproductor::getReproductor()->reproducirSonido(TIMERTICK);
			sono=true;
		}*/

	
	


	
}
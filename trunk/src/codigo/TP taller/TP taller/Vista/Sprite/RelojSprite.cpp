#include "RelojSprite.h"
#include "../../Modelo/Reproductor.h"


RelojSprite::RelojSprite(void)
{
}

RelojSprite::RelojSprite(SDL_Renderer* renderer, SDL_Rect recDestino, string path, int col, int fil, int anchoTex, int altoTex): DibujableTextura(){

	this->tiempoActual = 0;
	this->frame1 = 0;
	this->frame2 = 0;
	this->imagenReloj = spriteReloj;
	this->imagenRelojRojo = spriteRelojRojo;
	this->imagen = IMG_LoadTexture(renderer, path.c_str());
	this->contadorDePitidos = tiempoCritico;

	this->numCuadros = col*fil;
	int tamanioCuadroX = anchoTex / col;
	int tamanioCuadroY = altoTex / fil;
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
	
	if(tiempoActual == tiempoTurno){
		contadorDePitidos = tiempoCritico;
		setImagen(renderer, imagenReloj.c_str());
	}

	if(tiempoActual == tiempoCritico && contadorDePitidos == tiempoCritico)	setImagen(renderer, imagenRelojRojo.c_str());
	
	if(tiempoActual <= tiempoCritico && frame2 == contadorDePitidos) {
		Reproductor::getReproductor()->reproducirSonido(TIMERTICK);
		contadorDePitidos--;
	}

	frame1 = tiempoActual/10;
	frame2 = tiempoActual%10;
	
	SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[frame1], &rect);
	SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[frame2], &rect2);
	
}
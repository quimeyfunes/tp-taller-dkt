#include "GusanoSprite.h"


GusanoSprite::GusanoSprite(void)
{
}

GusanoSprite::GusanoSprite(SDL_Renderer* renderer, SDL_Rect recDestino, string path, int col, int fil, int anchoTex, int altoTex): DibujableTextura(){

	this->numCuadros = col*fil;
	this->velocidadRefresco = 60;
	this->contador = 0;

	int tamanioCuadroX = anchoTex / col;
	int tamanioCuadroY = altoTex / fil;
	this->frame = 0;
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
	this->cambiarImgDer = false;
	this->cambiarImgIzq = false;
	this->contIzq = 0;
	this->contDer = 0;
	this->contFrent = 0;
}

GusanoSprite::~GusanoSprite(void)
{
	delete []this->recCuadro;
}

void GusanoSprite::actualizar(Observable* observable) {

	Figura* fig = (Figura*)observable;
	//No se mueve
	if ( !(fig->seMueveALaDer() ) && !(fig->seMueveALaIzq()) ) {
		this->contFrent++;
		this->contIzq = 0;
		this->contDer = 0;
		this->setCambiarImgDer(false);
		this->setCambiarImgIzq(false);
		this->actualizarFrame();
	} else {
		if ((fig->seMueveALaDer())){
				this->contIzq = 0;
				this->contFrent = 0;
				this->contDer++;
				this->setCambiarImgDer(true);
				this->setCambiarImgIzq(false);
				this->actualizarFrame();
		} else {
			//Se mueve a la izquierda
			this->contDer = 0;
			this->contFrent = 0;
			this->contIzq++;
			this->setCambiarImgDer(false);
			this->setCambiarImgIzq(true);
			this->actualizarFrame();
		 }
	}

	SDL_Rect rect = this->rect;
	rect.x = (fig->getPosicion().x * relacionPPU) - rect.w /2;
	rect.y = (fig->getPosicion().y * relacionPPU) - rect.h /2;
	this->setRect(rect);
	
	/*this->contador++;
	if(this->contador >= this->velocidadRefresco){
		this->frame++;
		this->contador = 0;
	}

	if(this->frame >= this->numCuadros) this->frame = 0;*/

	/*SDL_Rect rectCartel = this->cartel->getRect();
	rectCartel.x = (fig->getPosicion().x * relacionPPU) - rect.w / 2;
	rectCartel.y = ((fig->getPosicion().y * relacionPPU) - rect.h / 2) - alturaCartel;
	this->cartel->setRect(rectCartel);*/


}

void GusanoSprite::dibujar(SDL_Renderer *renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx){

	SDL_Rect rect = this->rect;

	if ( !(this->hayCambioImgDer()) && !(this->hayCambioImgIzq()) && (this->contFrent == 1) ){
		this->setImagen(renderer, spriteWorm);
	}
	if ( (this->hayCambioImgDer()) && (this->contDer == 1 ) ){
		this->setImagen(renderer, rutaGusanoDer);
	}
	if ( (this->hayCambioImgIzq())  && (this->contIzq == 1) ){
		this->setImagen(renderer, rutaGusanoIzq);
	}

	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
		rect = realizarZoom(rect, corrimientoX, corrimientoY, escalaZoom);
		SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[frame], &rect);
	} else {
		rect.x -=corrimientoX;
		rect.y -=corrimientoY;
		SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[frame], &rect);
	}

	
	
}

void GusanoSprite::setCambiarImgDer(bool cambio){
	this->cambiarImgDer = cambio;
}

bool GusanoSprite::hayCambioImgDer(){
	return this->cambiarImgDer;
}

void GusanoSprite::setCambiarImgIzq(bool cambio){
	this->cambiarImgIzq = cambio;
}

bool GusanoSprite::hayCambioImgIzq(){
	return this->cambiarImgIzq;
}

void GusanoSprite::setNumCuadros(int numCuad){
	this->numCuadros;
}

void GusanoSprite::actualizarFrame(){

	this->contador++;
	if(this->contador >= this->velocidadRefresco){
			this->frame++;
			this->contador = 0;
	}

	if(this->frame >= this->numCuadros) this->frame = 0;
}

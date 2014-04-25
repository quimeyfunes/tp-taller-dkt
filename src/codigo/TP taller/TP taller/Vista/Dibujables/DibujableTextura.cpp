#include "DibujableTextura.h"

DibujableTextura::DibujableTextura(){}

DibujableTextura::DibujableTextura(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF) {
	this->rect = rect;
	this->imagen = IMG_LoadTexture(renderer, pathImagen.c_str());
	if(!imagen){
		Logger::getLogger()->escribir("No se encontró la textura " + pathImagen + ". Se usará la textura por defecto.");
		this->imagen = IMG_LoadTexture(renderer, pathDEF.c_str());
	}
	this->anguloRotacion = 0;
};

DibujableTextura::~DibujableTextura() {
	delete this->imagen;
}

SDL_Texture* DibujableTextura::getImagen(){
	return this->imagen;
}

SDL_Rect DibujableTextura::getRect(){
	return this->rect;
}

void DibujableTextura::setRect(SDL_Rect rect) {
	this->rect = rect;
}

int DibujableTextura::getAngulo() {
	return this->anguloRotacion;
}

void DibujableTextura::setAngulo(int angulo) {
	this->anguloRotacion = angulo;
}

void DibujableTextura::setColor(int* rgb, int a ){
	SDL_SetTextureColorMod(this->imagen,rgb[0],rgb[1],rgb[2]);
	SDL_SetTextureAlphaMod(this->imagen, a);
}

void DibujableTextura::dibujar(SDL_Renderer* renderer, int corrimiento, float escalaZoom, int posZoomX, int posZoomY){

	SDL_Rect rect = this->getRect();
	rect.x += corrimiento;
	
	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax) && (escalaZoom >= zoomMin)) {
		rect = this->realizarZoom(rect, posZoomX, posZoomY, escalaZoom);
		SDL_RenderCopyEx(renderer,this->getImagen(), NULL , &rect,this->getAngulo(),NULL,SDL_FLIP_NONE);
	} else {
		SDL_RenderCopyEx(renderer,this->getImagen(),NULL, &rect,this->getAngulo(),NULL,SDL_FLIP_NONE);
	}
	
}

DibujableSerializado DibujableTextura::getDibujableSerializado(int& tamano){
	DibujableSerializado serializado;
	return serializado;
}

SDL_Rect DibujableTextura::realizarZoom(SDL_Rect rect, int posX, int posY, float escalaZoom){

	SDL_Rect rectAux = rect;
	rectAux.x = ((rectAux.x * escalaZoom ) - posX);
	rectAux.y = ((rectAux.y  * escalaZoom)  - posY);
	rectAux.h = rectAux.h * escalaZoom;
	rectAux.w = rectAux.w * escalaZoom;

	return rectAux;


}
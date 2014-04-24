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

void DibujableTextura::dibujar(SDL_Renderer* renderer, int corrimiento, int escalaZoom, int posZoomX, int posZoomY){

	SDL_Rect rect = this->getRect();
	rect.x += corrimiento;
	int resize = 150;
	int newH = ((rect.h) * (resize / 100));
	int newW = ((rect.w) * (resize / 100));

	if ((escalaZoom != escalaZoomDefault) && (escalaZoom != zoomMax)) {

		int scrollLeft = (( posZoomX * (resize / 100 )) - ((newW / 2) /2));
		int scrollTop = ((posZoomY * (resize / 100)) - (( newH / 2 ) / 2));
		rect.x -= scrollLeft;
		rect.y -= scrollTop;
		rect.h = newH*escalaZoom;
		rect.w = newW*escalaZoom;
		SDL_RenderCopyEx(renderer,this->getImagen(), NULL , &rect,this->getAngulo(),NULL,SDL_FLIP_NONE);
	} else {
		SDL_RenderCopyEx(renderer,this->getImagen(),NULL, &rect,this->getAngulo(),NULL,SDL_FLIP_NONE);
	}
}

DibujableSerializado DibujableTextura::getDibujableSerializado(int& tamano){
	DibujableSerializado serializado;
	return serializado;
}
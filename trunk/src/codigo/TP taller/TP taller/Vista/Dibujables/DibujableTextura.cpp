#include "DibujableTextura.h"

DibujableTextura::DibujableTextura(){
	this->destruido = false;
}

DibujableTextura::DibujableTextura(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF) {
	this->rect = rect;
	this->imagen = IMG_LoadTexture(renderer, pathImagen.c_str());
	if(!imagen){
		Logger::getLogger()->escribir("No se encontró la textura " + pathImagen + ". Se usará la textura por defecto.");
		this->imagen = IMG_LoadTexture(renderer, pathDEF.c_str());
	}
	this->anguloRotacion = 0;
	this->destruido = false;
};

DibujableTextura::~DibujableTextura() {
	if(this->imagen != NULL){
		SDL_DestroyTexture(this->imagen);
		this->imagen = NULL;
	}
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

void DibujableTextura::setColor(int* rgb){
	SDL_SetTextureColorMod(this->imagen,rgb[0],rgb[1],rgb[2]);
}

void DibujableTextura::setTransparencia(int a){
	SDL_SetTextureAlphaMod(this->imagen, a);
}

void DibujableTextura::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int posZoomX, int posZoomY){

	SDL_Rect rect = this->rect;
	
	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
		rect = this->realizarZoom(this->rect, corrimientoX, corrimientoY, escalaZoom);
		SDL_RenderCopyEx(renderer,this->imagen, NULL , &rect,this->anguloRotacion,NULL,SDL_FLIP_NONE);
	} else {
		rect.x -= corrimientoX;
		rect.y -= corrimientoY;
		SDL_RenderCopyEx(renderer,this->imagen,NULL, &rect,this->anguloRotacion,NULL,SDL_FLIP_NONE);
	}
	
}

SDL_Rect DibujableTextura::realizarZoom(SDL_Rect rect, int corrimientoX, int corrimientoY, float escalaZoom){

	SDL_Rect rectAux = rect;

	rectAux.x = (rectAux.x  * escalaZoom ) - corrimientoX;
	rectAux.y = (rectAux.y  * escalaZoom)  - corrimientoY;
	rectAux.h = rectAux.h * escalaZoom;
	rectAux.w = rectAux.w * escalaZoom;

	return rectAux;


}

string DibujableTextura::serializar(){
	string serializado = "";
	return serializado;
}

void DibujableTextura::deserealizar(string aDeserealizar){
}

void DibujableTextura::setImagen(SDL_Renderer* renderer, string path){
	SDL_DestroyTexture(this->imagen);
	this->imagen = IMG_LoadTexture(renderer, path.c_str());
	if(!imagen){
		Logger::getLogger()->escribir("No se encontró la textura " + path + ". Se usará la textura por defecto.");
		this->imagen = IMG_LoadTexture(renderer, rutaGusanoDEF);
	}
}
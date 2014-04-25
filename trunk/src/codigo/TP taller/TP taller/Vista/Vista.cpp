#include "Vista.h"


Vista::Vista(EscenarioParseado* e){
	SDL_Init( SDL_INIT_EVERYTHING );
	this->window = SDL_CreateWindow("Worms!", 50, 50, e->anchoPx, e->altoPx,  SDL_WINDOW_SHOWN);
	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	this->listaDibujables = new list<Dibujable*>;
	this->anchoPx = e->anchoPx;
	this->altoPx = e->altoPx;
	this->corrimientoX = 0;
	this->corrimientoY = 0;
	SDL_SetWindowIcon(this->window, IMG_Load(rutaIcono));
	this->accion = JUGAR;
	this->escalaZoom = escalaZoomDefault;
	this->posZoomX = 0;
	this->posZoomY = 0;
}

Vista::Vista() {
}

Vista::~Vista() {

	delete this->window;
	delete this->renderer;
	delete this->listaDibujables;
	SDL_Quit();
}

SDL_Renderer* Vista::getRenderer() {
	return this->renderer;
}

void Vista::crearScrollingSprite(int x, int y, int ancho, int alto, string path){

	SDL_Rect rec;
	rec.x = x;
	rec.y = y;
	rec.w = ancho;
	rec.h = alto;
	ScrollingSprite* sprite = new ScrollingSprite(this->renderer, rec, path);
	this->agregarDibujable(sprite);	
}

Sprite* Vista::crearSprite(int x, int y, int anchoFrame, int altoFrame, string path, int col, int fil, int anchoTex, int altoTex){

	SDL_Rect recFrame;
	recFrame.x = x;
	recFrame.y = y;
	recFrame.w = anchoFrame;
	recFrame.h = altoFrame;
	Sprite* sprite = new Sprite(this->renderer, recFrame, path, col, fil, anchoTex, altoTex);
	this->agregarDibujable(sprite);	
	return sprite;
}

DibujableTextura* Vista::crearDibujableTextura(int x , int y ,int ancho,int alto, string pathImagen, string imagenDEF) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = ancho;
	rect.h = alto;
	
	DibujableTextura* dib = new DibujableTextura(this->renderer, rect, pathImagen, imagenDEF);
	this->agregarDibujable(dib);
	return dib;
}

CirculoDibujable* Vista::crearCirculoDibujable(int x , int y ,int radioHorizontal, int radioVertical) {
	CirculoDibujable* dib = new CirculoDibujable(x,y,radioHorizontal,radioVertical);
	this->agregarDibujable(dib);
	return dib;
}

PoligonoDibujable* Vista::crearPoligonoDibujable(short int n, float escalaAncho, float escalaAlto) {
	PoligonoDibujable* dib = new PoligonoDibujable(n,escalaAncho,escalaAlto);
	this->agregarDibujable(dib);
	return dib;
}

RectanguloDibujable* Vista::crearRectanguloDibujable(float ancho, float alto) {
	RectanguloDibujable* dib = new RectanguloDibujable(ancho,alto);
	this->agregarDibujable(dib);
	return dib;
}

GusanoDibujable* Vista::crearGusanoDibujable(int x, int y, int ancho, int alto, string pathImagen, string imagenDEF){

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = ancho;
	rect.h = alto;
	
	GusanoDibujable* dib = new GusanoDibujable(this->renderer, rect);
	this->agregarDibujable(dib);
	return dib;

}

void Vista::agregarDibujable(Dibujable* dibujable) {
	this->listaDibujables->push_back(dibujable);
}

list<Dibujable*>* Vista::getListaDibujables(){
	return this->listaDibujables;
	
}

void Vista::Dibujar(){
	SDL_RenderClear(this->renderer);
	for (list<Dibujable*>::iterator it = this->listaDibujables->begin(); it != this->listaDibujables->end(); it++) {
		(*it)->dibujar(this->renderer, this->corrimientoX, this->corrimientoY, this->escalaZoom, this->posZoomX, this->posZoomY);
	}
	SDL_RenderPresent(this->renderer);
}

int Vista::getAnchoPx() {
	return this->anchoPx;
}

int Vista::getAltoPx() {
	return this->altoPx;
}

bool Vista::leerEvento(SDL_Event* evento) {
	int x,y;
	//Para scroll
	SDL_GetMouseState(&x,&y);
	this->scroll(x,y);
	if (SDL_PollEvent(evento) != 0) {

		if(evento->type == SDL_QUIT){
			this->accion = SALIR;
			return true;
		}
		
		if(evento->type == SDL_KEYDOWN){

			switch(evento->key.keysym.sym){

			case SDLK_ESCAPE:	this->accion = SALIR;		return true;	break;
			case SDLK_s:		this->accion = JUGAR;		return true;	break;
			case SDLK_p:		this->accion = PAUSAR;		return true;	break;
			case SDLK_UP:		this->accion = ARRIBA;		return true;	break;
			case SDLK_LEFT:		this->accion = IZQUIERDA;	return true;	break;
			case SDLK_RIGHT:	this->accion = DERECHA;		return true;	break; 

			}
			
		} else {
			if (evento->type == SDL_KEYUP) {
				switch(evento->key.keysym.sym){
					case SDLK_UP:		this->accion = SOLTARARRIBA;		return true;	break;
					case SDLK_LEFT:		this->accion = SOLTARIZQUIERDA;		return true;	break;
					case SDLK_RIGHT:	this->accion = SOLTARDERECHA;		return true;	break; 
				}
			}
		}

		if (evento->type == SDL_MOUSEBUTTONDOWN) {
			this->accion = CLICK;
			return true;
		}

		if (evento->type == SDL_MOUSEWHEEL){
			this->zoom(evento,x,y);
		}
	}
	return false;
}

ACCION_REALIZADA Vista::getAccion(){
	return this->accion;
}

float Vista::getCorrimientoX(){
	return this->corrimientoX;
}

float Vista::getCorrimientoY(){
	return this->corrimientoY;
}

int Vista::getZoom(){
	return this->escalaZoom;
}

void Vista::setZoom(float escala){
	if (escala > zoomMax) {
		this->escalaZoom = zoomMax;
	} else 
		if (escala < zoomMin) {
			this->escalaZoom = zoomMin;
		} else {
			this->escalaZoom = escala;
		}
}

int Vista::getPosZoomX(){
	return this->posZoomX;
}

int Vista::getPosZoomY(){
	return this->posZoomY;
}

void Vista::scroll(int x , int y) {
	if ((x < (this->anchoPx * porcentajeScroll)) && ( x != 0)) {
		this->corrimientoX += this->anchoPx *velocidadScroll / x ;
	} else {
		if ((x > (this->anchoPx * (1 - porcentajeScroll))) && ( x != this->anchoPx)) {
			this->corrimientoX -= this->anchoPx * velocidadScroll / (this->anchoPx - x);
		} 
	}
	if ((y < (this->altoPx * porcentajeScroll)) && ( y != 0)) {
		this->corrimientoY += this->altoPx * velocidadScroll / y ;
	} else {
		if ((y > (this->altoPx * (1 - porcentajeScroll))) && ( y != this->altoPx)) {
			this->corrimientoY -= this->altoPx * velocidadScroll / (this->altoPx - y);
		} 
	}
	this->validarScroll();
}

void Vista::validarScroll() {
	//Proximamente... (?)
}

void Vista::zoom(SDL_Event* evento,int x, int y) {
	if (this->escalaZoom == zoomMin) {
		this->posZoomX = 0;
		this->posZoomY = 0;
	}
	this->posZoomX = (this->posZoomX + x ) / this->escalaZoom;
	this->posZoomY = (this->posZoomY + y ) / this->escalaZoom;
	if (evento->wheel.y > 0) {
		this->setZoom(this->escalaZoom + 0.25);
	} else {
		this->setZoom(this->escalaZoom - 0.25);
	}
	this->posZoomX = ((this->posZoomX * escalaZoom) - (this->anchoPx / 2));
	this->posZoomY = ((this->posZoomY * escalaZoom)  - (this->altoPx / 2));
	this->validarZoom();
}

void Vista::validarZoom() {
	if ((this->posZoomX + this->anchoPx) > (this->anchoPx * this->escalaZoom)) {
		this->posZoomX = this->anchoPx * (this->escalaZoom - 1);
	} else {
		if (this->posZoomX < 0) {
			this->posZoomX = 0;
		}
	}
	if ((this->posZoomY + this->altoPx) > (this->altoPx* this->escalaZoom)) {
		this->posZoomY = this->altoPx * (this->escalaZoom - 1);
	} else {
		if (this->posZoomY < 0) {
			this->posZoomY = 0;
		}
	}
}
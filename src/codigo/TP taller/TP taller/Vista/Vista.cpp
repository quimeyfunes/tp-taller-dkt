#include "Vista.h"


Vista::Vista(EscenarioParseado* e){
	SDL_Init( SDL_INIT_EVERYTHING );
	this->window = SDL_CreateWindow("Worms!", 50, 50, e->anchoPx, e->altoPx,  SDL_WINDOW_SHOWN );
	this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	this->listaDibujables = new list<Dibujable*>;
	this->anchoPx = e->anchoPx;
	this->altoPx = e->altoPx;
	this->corrimiento = 0;
	SDL_SetWindowIcon(this->window, IMG_Load(rutaIcono));
	this->accion = JUGAR;
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

void Vista::agregarDibujable(Dibujable* dibujable) {
	this->listaDibujables->push_back(dibujable);
}

list<Dibujable*>* Vista::getListaDibujables(){
	return this->listaDibujables;
	
}

void Vista::Dibujar(){
	SDL_RenderClear(this->renderer);
	for (list<Dibujable*>::iterator it = this->listaDibujables->begin(); it != this->listaDibujables->end(); it++) {
		(*it)->dibujar(this->renderer, this->corrimiento);
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
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	
	SDL_GetMouseState(&x,&y);
	if ((x < (this->anchoPx * porcentajeScroll)) && ( x != 0)) {
		this->corrimiento += this->anchoPx *velocidadScroll / x ;
	} else {
		if ((x > (this->anchoPx * (1 - porcentajeScroll))) && ( x != this->anchoPx)) {
			this->corrimiento -= this->anchoPx * velocidadScroll / (this->anchoPx - x);
		} 
	}
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
			
		}

		if (evento->type == SDL_MOUSEBUTTONDOWN) {
			this->accion = CLICK;
			return true;
		}
	}
	return false;
}

ACCION_REALIZADA Vista::getAccion(){
	return this->accion;
}

float Vista::getCorrimiento(){
	return this->corrimiento;
}

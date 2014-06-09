#include "Vista.h"

Vista::Vista(EscenarioParseado* e,SDL_Window* window, SDL_Renderer* renderer){
	this->anchoPx = e->anchoPx;
	this->altoPx = e->altoPx;
	this->anchoPxTot = e->anchoU * relacionPPU;
	this->altoPxTot = e->altoU * relacionPPU;
	this->window = window;
	SDL_SetWindowSize(this->window, this->anchoPx, this->altoPx);
	this->renderer = renderer;
	SDL_ShowWindow(window);
	this->listaDibujables = new list<Dibujable*>;
	this->corrimientoX = 0;
	this->corrimientoY = 0;
	this->accion = JUGAR;
	this->setZoom(escalaZoomDefault);

	this->motor = new MotorParticulas(renderer);
}

Vista::Vista() {
}

Vista::~Vista() {

	delete this->window;
	delete this->renderer;
	delete this->listaDibujables;
	SDL_Quit();
	TTF_Quit();
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

RelojSprite* Vista::crearRelojSprite(int x, int y, int anchoFrame, int altoFrame, string path, int col, int fil, int anchoTex, int altoTex){

	SDL_Rect recFrame;
	recFrame.x = x;
	recFrame.y = y;
	recFrame.w = anchoFrame;
	recFrame.h = altoFrame;
	RelojSprite* sprite = new RelojSprite(this->renderer, recFrame, path, col, fil, anchoTex, altoTex);
	this->agregarDibujable(sprite);	
	return sprite;

}

GusanoSprite* Vista::crearGusanoSprite(int x, int y, int anchoFrame, int altoFrame, string path, int col, int fil, int anchoTex, int altoTex,string nombre,int maximosClientes){

	SDL_Rect recFrame;
	recFrame.x = x;
	recFrame.y = y;
	recFrame.w = anchoFrame;
	recFrame.h = altoFrame;
	//Hardcodeo el nombre, nose si se va a seguir usando igual esto
	GusanoSprite* gusano = new GusanoSprite(this->renderer, recFrame, path, col, fil, anchoTex, altoTex, nombre,maximosClientes);
	this->agregarDibujable(gusano);	
	return gusano;
}

CartelInfo* Vista::crearCartelInfo(int x, int y, int ancho, int alto){
	SDL_Rect rec;
	rec.x=x;
	rec.y=y;
	rec.h=alto;
	rec.w=ancho;

	CartelInfo* cartelInfo = new CartelInfo(this->renderer, rec);
	this->agregarDibujable(cartelInfo);
	return cartelInfo;
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
	
	GusanoDibujable* dib = new GusanoDibujable(this->renderer, rect,pathImagen,imagenDEF);
	CartelDibujable* dib2 = dib->getCartel();

	/*SDL_Rect rectCart = dib2->getRect();
	rectCart.x = rect.x;
	rectCart.y = rect.y + 80;
	rectCart.w = rect.w;
	rectCart.h = rect.h / 4;*/

	this->agregarDibujable(dib);
	this->agregarDibujable(dib2);
	return dib;

}

ATiempoDibujable* Vista::crearArmaTiempoDibujable(int x, int y, int ancho, int alto, string pathImagen, string imagenDef){

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = ancho;
	rect.h = alto;

	ATiempoDibujable* dib = new ATiempoDibujable(this->renderer, rect,pathImagen,imagenDef);
	this->agregarDibujable(dib);
	return dib;
}


AContactoDibujable* Vista::crearArmaContactoDibujable(int x, int y, int ancho, int alto, string pathImagen, string imagenDef){

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = ancho;
	rect.h = alto;

	AContactoDibujable* dib = new AContactoDibujable(this->renderer, rect,pathImagen,imagenDef);
	this->agregarDibujable(dib);
	return dib;
}


PanelArmas* Vista::crearPanelArmas(int x , int y, int ancho, int alto, vector<int> armas) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = ancho;
	rect.h = alto;
	
	PanelArmas* dib = new PanelArmas(this->renderer, rect, "imagenes/texturas/armas/panelFondo2.png", "");
	this->agregarDibujable(dib);

	//Tengo que recorrrer el vector y agregar las armas
	
	dib->agregarArma(this->renderer,"imagenes/texturas/armas/bazooka.1.png","imagenes/texturas/armas/bazooka.2.png",-1,false);
	dib->agregarArma(this->renderer,"imagenes/texturas/armas/cluster.1.jpg","imagenes/texturas/armas/cluster.2.jpg",4,false);
	dib->agregarArma(this->renderer,"imagenes/texturas/armas/hgrenade.1.png","imagenes/texturas/armas/hgrenade.2.png",5,false);
	dib->agregarArma(this->renderer,"imagenes/texturas/armas/dynamite.1.png","imagenes/texturas/armas/dynamite.2.png",4,false);
	dib->agregarArma(this->renderer,"imagenes/texturas/armas/banana.1.png","imagenes/texturas/armas/banana.2.png",3,false);
	dib->agregarArma(this->renderer,"imagenes/texturas/armas/misiles.1.jpg","imagenes/texturas/armas/misiles.2.jpg",2,true);
	dib->agregarArma(this->renderer,"imagenes/texturas/armas/suicida.1.jpg","imagenes/texturas/armas/suicida.2.jpg",2,false);

	return dib;
}


void Vista::agregarDibujable(Dibujable* dibujable) {
	this->listaDibujables->push_back(dibujable);
}

list<Dibujable*>* Vista::getListaDibujables(){
	return this->listaDibujables;
}

void Vista::setListaDibujables(list<Dibujable*>* dibujables){
	this->listaDibujables = dibujables;
}

void Vista::Dibujar(){
	SDL_RenderClear(this->renderer);
	list<Dibujable*>::iterator it = this->listaDibujables->begin();
	while (it != this->listaDibujables->end())
	{
		if ((*it)->getDestruido()) 	{
			delete (*it);
			this->listaDibujables->erase(it++);  
		} else {
			(*it)->dibujar(this->renderer, this->corrimientoX, this->corrimientoY, this->escalaZoom, this->anchoPx, this->altoPx);
			++it;
		}
	}
	this->motor->dibujar(this->renderer, this->corrimientoX, this->corrimientoY, this->escalaZoom, this->anchoPx, this->altoPx);
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
			case SDLK_DOWN:		this->accion = ABAJO;		return true;	break;
			case SDLK_SPACE:	this->accion = ESPACIO;		return true;	break;
			case SDLK_RETURN:	this->accion = ENTER;		return true;	break;

			}
			
		} else {
			if (evento->type == SDL_KEYUP) {
				switch(evento->key.keysym.sym){
					case SDLK_UP:		this->accion = SOLTARARRIBA;		return true;	break;
					case SDLK_DOWN:		this->accion = SOLTARABAJO;			return true;	break;
					case SDLK_LEFT:		this->accion = SOLTARIZQUIERDA;		return true;	break;
					case SDLK_RIGHT:	this->accion = SOLTARDERECHA;		return true;	break; 
					case SDLK_SPACE:    this->accion = SOLTARESPACIO;		return true;	break;
					case SDLK_RETURN:	this->accion = SOLTARENTER;			return true;	break;
				}
			}
		}

		if (evento->type == SDL_MOUSEBUTTONDOWN) {
			switch(evento->button.button){
			case SDL_BUTTON_LEFT:		this->accion = CLICK;			return true;	break;
			case SDL_BUTTON_RIGHT:		this->accion = CLICKDERECHO;	return true;	break;
			}
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

float Vista::getZoom(){
	return this->escalaZoom;
}

void Vista::setZoom(float escala){
	if (escala > zoomMax) {
		this->escalaZoom = zoomMax;
		return;
	} 
	float zoomMinAncho = this->anchoPx * 1.0 / this->anchoPxTot;
	float zoomMinAlto = this->altoPx * 1.0 / this->altoPxTot;
	if ((zoomMinAlto > zoomMinAncho) && (escala < zoomMinAlto)){
		this->escalaZoom = zoomMinAlto;
		return;
	}
	if ((zoomMinAlto <= zoomMinAncho) && (escala < zoomMinAncho)){
		this->escalaZoom = zoomMinAncho;
		return;
	}
	// Cuando pasa a zoomMin termina con un numero (generalmente) no multiplo de 0.25. Aca lo vuelvo a multiplo de 0.25. 
	int esc = escala * 100;
	if (esc % 25 != 0) { 
		this->escalaZoom = (esc / 25) * 0.25;
		return;
	}
	this->escalaZoom = escala;
	
}

void Vista::scroll(int x , int y) {
	if ((x < (this->anchoPx * porcentajeScroll)) && ( x != 0)) {
		this->corrimientoX -= this->anchoPx * velocidadScroll / x ;
	} else {
		if ((x > (this->anchoPx * (1 - porcentajeScroll))) && ( x < (this->anchoPx - 1))) {
			this->corrimientoX += this->anchoPx * velocidadScroll / (this->anchoPx - x);
		} 
	}
	if ((y < (this->altoPx * porcentajeScroll)) && ( y != 0)) {
		this->corrimientoY -= this->altoPx * velocidadScroll / y ; 
	} else {
		if ((y > (this->altoPx * (1 - porcentajeScroll))) && ( y < (this->altoPx - 1))) {
			this->corrimientoY += this->altoPx * velocidadScroll / (this->altoPx - y);
		} 
	}
	this->validarCorrimiento();
}

void Vista::zoom(SDL_Event* evento,int x, int y) {

	float escalaAnterior = this->escalaZoom;
		
	if (evento->wheel.y > 0) {
		this->setZoom(this->escalaZoom + 0.25);
	} else {
		this->setZoom(this->escalaZoom - 0.25);
	}

	if (this->escalaZoom != escalaAnterior) {
		this->corrimientoX = (this->corrimientoX + x ) / escalaAnterior;
		this->corrimientoY = (this->corrimientoY + y ) / escalaAnterior;
		this->corrimientoX = (this->corrimientoX * escalaZoom) - this->anchoPx / 2;
		this->corrimientoY = (this->corrimientoY * escalaZoom)  - this->altoPx / 2;
	}
	this->validarCorrimiento();
}

void Vista::validarCorrimiento() {
	if ((this->corrimientoX + this->anchoPx) > this->anchoPxTot * escalaZoom) {
		this->corrimientoX = this->anchoPxTot * escalaZoom - this->anchoPx;
	} else {
		if (this->corrimientoX < 0) {
			this->corrimientoX = 0;
		}
	}
	if ((this->corrimientoY + this->altoPx) > this->altoPxTot * escalaZoom) {
		this->corrimientoY = this->altoPxTot * escalaZoom- this->altoPx;
	} else {
		if (this->corrimientoY < 0) {
			this->corrimientoY = 0;
		}
	}
}

void Vista::destruir(float x,float y,int radio, LectorTerreno* lector){
	
	lector->destruirMatriz(x, y, radio);
	int i = 0;
	//El terreno es 5to
	for (list<Dibujable*>::iterator it = this->listaDibujables->begin(); it != this->listaDibujables->end() && i < 5; it++) {
		if ( i == 4) {
			DibujableTextura* d = (DibujableTextura*) (*it);
			d->setImagen(this->renderer,lector->getRutaTexturaActualizada());
		}
		i++;
	}
}


DibujableTerreno* Vista::crearDibujableTerreno(int x , int y ,int ancho,int alto, string pathImagen, string imagenDEF) {
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = ancho;
	rect.h = alto;
	
	DibujableTerreno* dib = new DibujableTerreno(this->renderer, rect, pathImagen, imagenDEF);
	this->agregarDibujable(dib);
	return dib;
}

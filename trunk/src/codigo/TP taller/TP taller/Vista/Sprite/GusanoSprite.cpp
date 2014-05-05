#include "GusanoSprite.h"


GusanoSprite::GusanoSprite(void)
{
	this->recCuadro = NULL;
	this->cartel = NULL;
	this->imagen = NULL;
}

GusanoSprite::GusanoSprite(SDL_Renderer* renderer, SDL_Rect recDestino, string path, int col, int fil, int anchoTex, int altoTex, string nombre): DibujableTextura(){

	this->numCuadros = col*fil;
	this->velocidadRefresco = timeGusanoQuieto;
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
	this->contMuerte = 0;
	this->estado = IZQ;
	
	SDL_Rect rectCart = rect;
	rectCart.w = rect.w;
	rectCart.h = rect.h / 4;
	//this->cartel = NULL;
	this->cartel = new CartelDibujable(renderer, rectCart, rutaCartel, rutaCartelDEF, nombre);
}

GusanoSprite::~GusanoSprite(void)
{
	if(this->recCuadro != NULL){
		delete []this->recCuadro;
	}

	/*if(this->cartel != NULL){
		delete this->cartel;
	}*/
	if(this->imagen != NULL){
		SDL_DestroyTexture(this->imagen);
		this->imagen = NULL;
	}
}

void GusanoSprite::actualizar(Observable* observable) {

	Figura* fig = (Figura*)observable;
	
	if (!(fig->estaMuerto())){
		this->contMuerte = 0;
		//No se mueve
		if ( !(fig->seMueveALaDer() ) && !(fig->seMueveALaIzq()) ) {
			this->contFrent++;
			this->contIzq = 0;
			this->contDer = 0;
			this->setCambiarImgDer(false);
			this->setCambiarImgIzq(false);
			this->velocidadRefresco = timeGusanoQuieto;
			this->actualizarFrame();
		} else {
			this->velocidadRefresco = timeGusanoMovil;
				if ((fig->seMueveALaDer())){
					this->contIzq = 0;
					this->contFrent = 0;
					this->contDer++;
					this->setCambiarImgDer(true);
					this->setCambiarImgIzq(false);
					this->actualizarFrame();
					this->estado = DER;
				} else {
					//Se mueve a la izquierda
					this->contDer = 0;
					this->contFrent = 0;
					this->contIzq++;
					this->setCambiarImgDer(false);
					this->setCambiarImgIzq(true);
					this->actualizarFrame();
					this->estado = IZQ;
				}
			}
	} else {
		this->velocidadRefresco = timeGrave;
		this->contIzq = 0;
		this->contDer = 0;
		this->contFrent = 0;
		this->contMuerte++;
		this->actualizarFrame();
		this->estado = MUERTO;	
	}

	SDL_Rect rect = this->rect;
	rect.x = (fig->getPosicion().x * relacionPPU) - rect.w /2;
	rect.y = (fig->getPosicion().y * relacionPPU) - rect.h /2;
	this->setRect(rect);

	SDL_Rect rectCartel = this->cartel->getRect();
	rectCartel.x = (fig->getPosicion().x * relacionPPU) - rectCartel.w / 2;
	rectCartel.y = ((fig->getPosicion().y * relacionPPU) - rect.h / 2) - alturaCartel;
	this->cartel->setRect(rectCartel);
}

void GusanoSprite::dibujar(SDL_Renderer *renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx){

	SDL_Rect rect = this->rect;

	if ((this->estado == MUERTO) && (this->contMuerte == 1)){
		this->setImagen(renderer, rutaGrave);
	} else {
		if ( !(this->hayCambioImgDer()) && !(this->hayCambioImgIzq()) && (this->contFrent == 1) ){
			this->setFrame(0);
				if (this->estado == IZQ)
					this->setImagen(renderer, spriteWormIzq);
				else
					this->setImagen(renderer, spriteWormDer);
		}
		if ( (this->hayCambioImgDer()) && (this->contDer == 1 ) ){
			if (this->estado == MUERTO){
				this->setImagen(renderer, rutaGrave);
			} else {
				this->setImagen(renderer, rutaGusanoDer);
			}
		}
		if ( (this->hayCambioImgIzq())  && (this->contIzq == 1) ){
			if (this->estado == MUERTO){
				this->setImagen(renderer, rutaGrave);
			} else {
				this->setImagen(renderer, rutaGusanoIzq);
			}
		}
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

int GusanoSprite::getFrame(){
	return this->frame;
}

void GusanoSprite::setFrame(int frame){
	this->frame = frame;
}

string GusanoSprite::serializar(){
	string serializado = "";
	serializado = StringUtil::int2string(serializadoGusanoSprite);
	serializado += separadorCampoTipoEntidades;    
	serializado += StringUtil::float2string(this->frame);
	serializado += separadorCamposEntidades;    
	serializado += StringUtil::int2string(this->rect.x);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->rect.y);
    serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->rect.w);
	serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->rect.h);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->contDer);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->contIzq);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->cambiarImgDer);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->cambiarImgIzq);
	return serializado;
}

void GusanoSprite::deserealizar(string aDeserealizar){
	vector<string> des = StringUtil::split(aDeserealizar,separadorCampoTipoEntidades);
	//des.at(0) tiene el tipo, des.at(0) tiene el resto de los atributos
	vector<string> atributos = StringUtil::split(des.at(1),separadorCamposEntidades);
	this->frame = StringUtil::str2float(atributos.at(0).c_str());
    SDL_Rect rectAux;
	rectAux.x = StringUtil::str2int(atributos.at(1));
	rectAux.y = StringUtil::str2int(atributos.at(2));
	rectAux.w = StringUtil::str2int(atributos.at(3));
	rectAux.h = StringUtil::str2int(atributos.at(4));
	this->setRect(rectAux);
	this->contDer = StringUtil::str2int(atributos.at(5));
	this->contIzq = StringUtil::str2int(atributos.at(6));
	this->cambiarImgDer = StringUtil::str2int(atributos.at(7));
	this->cambiarImgIzq = StringUtil::str2int(atributos.at(8));
	this->recCuadro = NULL;
	this->cartel = NULL;
	this->imagen = NULL;
}

CartelDibujable* GusanoSprite::getCartel(){
	return this->cartel;
}
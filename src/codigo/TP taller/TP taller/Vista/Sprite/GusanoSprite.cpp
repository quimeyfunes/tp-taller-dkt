#include "GusanoSprite.h"


GusanoSprite::GusanoSprite(void)
{
	this->recCuadro = NULL;
	this->cartel = NULL;
	this->imagen = NULL;
	this->vida = NULL;
}

GusanoSprite::GusanoSprite(SDL_Renderer* renderer, SDL_Rect recDestino, string path, int col, int fil, int anchoTex, int altoTex, string nombre,int maximosCLientes): DibujableTextura(){
	
	
	this->armaTipo = NINGUNA;
	this->numCuadros = col*fil;
	this->velocidadRefresco = timeGusanoQuieto;
	this->contador = 0;
	this->crosshair = IMG_LoadTexture(renderer, rutaCrosshair);
	this->mostrarCrosshair = false;
	this->frameCrosshair = 0;
	this->posFigura = new SDL_Point();
	
	int tamanioCuadroX = anchoTex / col;
	int tamanioCuadroY = altoTex / fil;
	this->frame = 0;
	this->rect = recDestino;

	this->rectApuntando = new SDL_Rect[32];
	for(int i=0; i< 32; i++){
		rectApuntando[i].h = 60;
		rectApuntando[i].w = 60;
		rectApuntando[i].x = 0;
		rectApuntando[i].y = i* 60;
	}

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

	this->recPotencia = new SDL_Rect[17];
	for(int i=0; i<17; i++){
		this->recPotencia[i].h = 28;
		this->recPotencia[i].w = 244;
		this->recPotencia[i].x = 0;
		this->recPotencia[i].y = i*28;
	}
	
	this->enUso = recCuadro;
	this->imagen = IMG_LoadTexture(renderer, path.c_str());
	this->cambiarImgDer = false;
	this->cambiarImgIzq = false;
	this->contIzq = 0;
	this->contDer = 0;
	this->contFrent = 0;
	this->contMuerte = 0;
	this->contArma = 0;
	this->estado = IZQ;
	this->nombre = nombre;
	SDL_Rect rectCart = this->rect;
	rectCart.h = rect.h / 4;
	rectCart.x = this->rect.x + this->rect.w/2;
	this->maximosCLientes = maximosCLientes;
	
	//this->cartel = NULL;
	this->cartel = new CartelDibujable(renderer, rectCart, rutaCartel, rutaCartelDEF, this->nombre);
	
	//this->mostrarCartel = false;
	for(int i=0; i < this->maximosCLientes; i++){
		this->mostrarCartel.push_back(false);
	}
	this->cliente = 0;
	SDL_Rect rectVida = this->rect;
	rectVida.h = 5;
	rectVida.w = pxPorVida * vidaGusano;
	this->vida = new DibujableTextura(renderer,rectVida,rutaVida,rutaVida);
	int rgb[3];
	rgb[0] = 0;
	rgb[1] = 255;
	rgb[2] = 0;
	this->vida->setColor(rgb);
}

GusanoSprite::~GusanoSprite(void)
{
	if(this->recCuadro != NULL){
		delete []this->recCuadro;
	}

	if(this->cartel != NULL){
		delete this->cartel;
	}
	if(this->imagen != NULL){
		SDL_DestroyTexture(this->imagen);
		this->imagen = NULL;
	}
}

void GusanoSprite::actualizar(Observable* observable) {

	Gusano* fig = (Gusano*)observable;
	this->posFigura->x = fig->getBody()->GetPosition().x;
	this->posFigura->y = fig->getBody()->GetPosition().y;

	if (!(fig->estaMuerto())){
		this->contMuerte = 0;
		//No se mueve
		this->frameCrosshair = 0;

		if ( !(fig->seMueveALaDer() ) && !(fig->seMueveALaIzq()) ) {
			
				this->contIzq = 0;
				this->contDer = 0;
				this->setCambiarImgDer(false);
				this->setCambiarImgIzq(false);
			if(!fig->tieneUnArma()){
				this->mostrarCrosshair = false;
				this->contArma =0;
				this->contFrent++;
				this->enUso = recCuadro;
				this->armaTipo = NINGUNA;
				this->velocidadRefresco = timeGusanoQuieto;
				this->actualizarFrame();
			}else{
				this->contFrent = 0;
				this->contArma++;
					//printf("HOLA\n");
					this->enUso = rectApuntando;
					this->armaTipo = fig->getTipoArma();
					if(armaTipo == DINAMITA){
						this->frame = 0;
					}else{
						this->actualizarFrameCrosshair(fig->armaActual.potenciaDisparo);
						this->actualizarFrameDisparo(fig->armaActual.anguloDisparo);
					}
					//cout<<this->frameDisparo<<endl;
			}
		} else {
			this->velocidadRefresco = timeGusanoMovil;
			this->armaTipo = NINGUNA;
			this->mostrarCrosshair = false;
				if ((fig->seMueveALaDer())){	
					this->contIzq = 0;
					this->contFrent = 0;
					this->contArma = 0;
					this->contDer++;
					this->setCambiarImgDer(true);
					this->setCambiarImgIzq(false);
					this->actualizarFrame();
					this->estado = DER;
				} else {
					//Se mueve a la izquierda
					this->contDer = 0;
					this->contFrent = 0;
					this->contArma = 0;
					this->contIzq++;
					this->setCambiarImgDer(false);
					this->setCambiarImgIzq(true);
					this->actualizarFrame();
					this->estado = IZQ;
				}
			}
	} else {
		this->mostrarCrosshair=false;
		this->velocidadRefresco = timeGrave;
		this->contIzq = 0;
		this->contDer = 0;
		this->contFrent = 0;
		this->contMuerte++;
		//this->actualizarFrame();
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

	for(int i=0; i < this->maximosCLientes; i++){
		this->mostrarCartel[i] = fig->getMeClickearon(i);
	}

	SDL_Rect rectVida = this->vida->getRect();
	
	rectVida.y = this->rect.y - 10;
	int vida = fig->getVida() * pxPorVida;
	float fraccion = fig->getVida() * 1.0 / vidaGusano;
	if (vida != rectVida.w) {
		rectVida.w = vida;
		int rgb[3];
		rgb[0] = (1 - fraccion) * 255 / 0.5;
		if (rgb[0] > 255) rgb[0] = 255;
		if (fraccion < 0.5) {
			rgb[1] = fraccion / 2 * 255;
		} else {
			rgb[1] = 255;
		}
		rgb[2] = 0;
		this->vida->setColor(rgb);
	}
	rectVida.x = this->rect.x + this->rect.w/2 - rectVida.w/2;
	this->vida->setRect(rectVida);
	/*if (fig->getMeClickearon()) {
		this->mostrarCartel = true;
	} else {
		this->mostrarCartel = false;
	}*/

	this->congelado = fig->getCongelado();
}

void GusanoSprite::dibujar(SDL_Renderer *renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx){
	SDL_Rect rect = this->rect;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	
	if (this->estado == DER) {
			flip = SDL_FLIP_HORIZONTAL;
	}

	if ((this->estado == MUERTO) && (this->contMuerte == 1)){
		this->setFrame(0);
		this->setImagen(renderer, rutaGrave);
	} else {
		if ( !(this->hayCambioImgDer()) && !(this->hayCambioImgIzq()) && ((this->contFrent == 1) || this->contArma == 1) ){ //Esta quieto
			this->setFrame(0);
			if(this->congelado){
				this->setImagen(renderer, rutaWormGrisIzq);
			}else{
				switch(this->armaTipo){
				case NINGUNA:	this->setImagen(renderer, spriteWormIzq);	this->mostrarCrosshair = false;	 break;
				case BAZOOKA:	this->setImagen(renderer, rutaWormBaz);		this->mostrarCrosshair = true;	 break;
				case GRANADA:	this->setImagen(renderer, rutaWormGran);	this->mostrarCrosshair = true;	 break;
				case ALELUYA:	this->setImagen(renderer, rutaWormAle);		this->mostrarCrosshair = true;	 break;
				case DINAMITA:	this->setImagen(renderer, rutaWormDin);		this->mostrarCrosshair = false;	 break;
				case BANANA:	this->setImagen(renderer, rutaWormBan);		this->mostrarCrosshair = true;	 break;
				}
			}
		} else {
			if ((this->hayCambioImgDer() && this->contDer == 1) || (this->hayCambioImgIzq() && this->contIzq == 1)) {
				this->setImagen(renderer, rutaGusanoIzq);
				
			}
		}
	}
	
	if ((escalaZoom != escalaZoomDefault) && (escalaZoom <= zoomMax)) {
		rect = realizarZoom(rect, corrimientoX, corrimientoY, escalaZoom);
	} else {
		rect.x -=corrimientoX;
		rect.y -=corrimientoY;
	}

	SDL_RenderCopyEx(renderer, this->imagen, &this->enUso[frame], &rect,0,NULL,flip);
		if(this->mostrarCrosshair && this->estado != MUERTO){
			SDL_Rect aux = rect;
			aux.w = 244 * escalaZoom;
			aux.h = 28 * escalaZoom;
			aux.x -= (128 - (1.5f * anchoGusano)*relacionPPU);
			aux.y += altoGusano/2 * relacionPPU;

			if(this->estado == DER)
				SDL_RenderCopyEx(renderer, this->crosshair, &this->recPotencia[frameCrosshair], &aux, -this->anguloDisparo, 0, flip);
			else
				SDL_RenderCopyEx(renderer, this->crosshair, &this->recPotencia[frameCrosshair], &aux, this->anguloDisparo, 0, flip);
		}


	if (this->mostrarCartel[this->cliente] && this->estado != MUERTO) {
		this->cartel->dibujar(renderer,corrimientoX,corrimientoY,escalaZoom,anchoPx,altoPx);
	}
	this->vida->dibujar(renderer,corrimientoX,corrimientoY,escalaZoom,anchoPx,altoPx);
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

void GusanoSprite::actualizarFrameDisparo(int angulo){

	this->frame = (float)(15.5/90) * angulo + 15.5f;
	this->anguloDisparo = angulo;
}

void GusanoSprite::actualizarFrameCrosshair(int potencia){
	
	this->frameCrosshair = ((float)(17.0f/POTENCIA_MAXIMA_DISPARO)) * potencia;
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
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->contFrent);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->contMuerte);
	serializado += separadorCamposEntidades;
	serializado += this->nombre;
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->contador);
	serializado += separadorCamposEntidades;
	
	int estadoNumero = 1;
	if(this->estado == DER){
		estadoNumero = 2;
	}
	else if( this->estado == MUERTO){
		estadoNumero = 3;
	}
	serializado += StringUtil::int2string(estadoNumero);

	serializado += separadorCamposEntidades;
	//serializado += StringUtil::int2string(this->mostrarCartel);
	std::stringstream ss;
	for(size_t j = 0; j < this->mostrarCartel.size(); ++j)
	{
		if(j != 0){
			ss << separadorCamposArreglo;
		}
		ss << StringUtil::int2string(this->mostrarCartel[j]);
	}
	std::string vectorSerializado = ss.str();
	serializado += vectorSerializado;
	
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->velocidadRefresco);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->numCuadros);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->congelado);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->armaTipo);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->contArma);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->anguloDisparo);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->anguloRotacion);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->mostrarCrosshair);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->frameCrosshair);

	/*serializado += separadorCamposEntidades;    
	serializado += StringUtil::int2string(this->recPotencia->x);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->recPotencia->y);
    serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->recPotencia->w);
	serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->recPotencia->h);

	serializado += separadorCamposEntidades;    
	serializado += StringUtil::int2string(this->enUso->x);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->enUso->y);
    serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->enUso->w);
	serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->enUso->h);*/
	
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

	bool camb = false;
	int cambNumero = StringUtil::str2int(atributos.at(7));
	if(cambNumero > 0){
		camb = true;
	}
	this->cambiarImgDer = camb;
	
	camb = false;
	cambNumero = StringUtil::str2int(atributos.at(8));
	if(cambNumero > 0){
		camb = true;
	}
	this->cambiarImgIzq = camb;

	this->contFrent = StringUtil::str2int(atributos.at(9));
	this->contMuerte = StringUtil::str2int(atributos.at(10));
	this->nombre = atributos.at(11);
	this->contador = StringUtil::str2int(atributos.at(12));

	int estadoNumero = StringUtil::str2int(atributos.at(13));
	if(estadoNumero == 1){
		this->estado = IZQ;
	}
	else if(estadoNumero == 2){
		this->estado = DER;
	}
	else if(estadoNumero == 3){

		this->estado = MUERTO;
	}	

	//this->mostrarCartel = StringUtil::str2int(atributos.at(14));
	vector<string> mostrarCartelArreglo = StringUtil::split(atributos.at(14),separadorCamposArreglo);
	for (int i = 0; i < mostrarCartelArreglo.size(); i++) {
		this->mostrarCartel.push_back(StringUtil::str2int(mostrarCartelArreglo.at(i)));
	}

	this->velocidadRefresco = StringUtil::str2int(atributos.at(15));
	this->numCuadros = StringUtil::str2int(atributos.at(16));
	this->congelado = StringUtil::str2int(atributos.at(17));
	this->armaTipo = static_cast<tipoArma>(StringUtil::str2int(atributos.at(18)));
	this->contArma = StringUtil::str2int(atributos.at(19));
	this->anguloDisparo = StringUtil::str2int(atributos.at(20));
	this->anguloRotacion = StringUtil::str2int(atributos.at(21));
	this->mostrarCrosshair = StringUtil::str2int(atributos.at(22));
	this->frameCrosshair = StringUtil::str2int(atributos.at(23));

	/*SDL_Rect* rectAux2 = new SDL_Rect();
	rectAux2->x = StringUtil::str2int(atributos.at(24));
	rectAux2->y = StringUtil::str2int(atributos.at(25));
	rectAux2->w = StringUtil::str2int(atributos.at(26));
	rectAux2->h = StringUtil::str2int(atributos.at(27));
	this->recPotencia = rectAux2;

	SDL_Rect* rectAux3 = new SDL_Rect();
	rectAux3->x = StringUtil::str2int(atributos.at(28));
	rectAux3->y = StringUtil::str2int(atributos.at(29));
	rectAux3->w = StringUtil::str2int(atributos.at(30));
	rectAux3->h = StringUtil::str2int(atributos.at(31));
	this->enUso = rectAux3;*/

	this->recCuadro = NULL;
	this->cartel = NULL;
	this->imagen = NULL;
}

CartelDibujable* GusanoSprite::getCartel(){
	return this->cartel;
}

string GusanoSprite::getNombre(){
	return this->nombre;
}
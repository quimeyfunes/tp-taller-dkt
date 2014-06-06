#include "RelojArmaSprite.h"


RelojArmaSprite::RelojArmaSprite(void)
{
	this->recCuadro = NULL;
	this->imagen = NULL;
}

RelojArmaSprite::RelojArmaSprite(SDL_Renderer* renderer, SDL_Rect recDestino, string path): DibujableTextura(){

	this->frame = 0;
	this->imagen = IMG_LoadTexture(renderer, spriteRelojRojo);

	this->numCuadros = 10;
	int tamanioCuadroX = 42;
	int tamanioCuadroY = 560 / 10;
	this->rect = recDestino;
	this->recCuadro = new SDL_Rect[numCuadros];

	for(int i = 0; i < this->numCuadros; i++){
		recCuadro[i].h = tamanioCuadroY;
		recCuadro[i].w = tamanioCuadroX;
		recCuadro[i].x = 0;
		recCuadro[i].y = i * tamanioCuadroY;
	}
}

RelojArmaSprite::~RelojArmaSprite(void)
{
	if(this->recCuadro != NULL){
		delete []this->recCuadro;
	}
}

void RelojArmaSprite::setTiempoRestante(int tiempo){
	if (tiempo < 0) {
		this->tiempoActual = 0;
	} else {
		this->tiempoActual = tiempo;
	}
}

void RelojArmaSprite::dibujar(SDL_Renderer *renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx){
	
	SDL_Rect rect = realizarZoom(this->rect,corrimientoX,corrimientoY,escalaZoom);
	//if ((this->tiempoActual > 9) || (this->tiempoActual)) this->tiempoActual = 9;
	SDL_RenderCopy(renderer, this->imagen, &this->recCuadro[this->tiempoActual], &rect);
}


string RelojArmaSprite::serializar(){
	string serializado = "";
	serializado = StringUtil::int2string(serializadoRelogArmaSprite);
	serializado += separadorCampoTipoEntidades;
	serializado += StringUtil::int2string(this->rect.x);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->rect.y);
    serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->rect.w);
	serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->rect.h);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->frame);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->numCuadros);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->tiempoActual);

	return serializado;
}

void RelojArmaSprite::deserealizar(string aDeserealizar){
	vector<string> des = StringUtil::split(aDeserealizar,separadorCampoTipoEntidades);
	//des.at(0) tiene el tipo, des.at(0) tiene el resto de los atributos
	vector<string> atributos = StringUtil::split(des.at(1),separadorCamposEntidades);
    SDL_Rect rectAux;
	rectAux.x = StringUtil::str2int(atributos.at(0));
	rectAux.y = StringUtil::str2int(atributos.at(1));
	rectAux.w = StringUtil::str2int(atributos.at(2));
	rectAux.h = StringUtil::str2int(atributos.at(3));
	this->setRect(rectAux);

	this->frame = StringUtil::str2int(atributos.at(4));
	this->numCuadros = StringUtil::str2int(atributos.at(5));
	this->tiempoActual = StringUtil::str2int(atributos.at(6));
}
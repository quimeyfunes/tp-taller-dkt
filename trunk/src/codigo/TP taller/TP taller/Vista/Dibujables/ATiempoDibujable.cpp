#include "ATiempoDibujable.h"


ATiempoDibujable::ATiempoDibujable(void)
{
	this->reloj = new RelojArmaSprite();
	SDL_Rect rectReloj;
	rectReloj.w = 10;
	rectReloj.h = 10;
	this->reloj->setRect(rectReloj);

}

ATiempoDibujable::ATiempoDibujable(SDL_Renderer* renderer, SDL_Rect rect, string pathImagen, string pathDEF)
	: ArmaDibujable(renderer, rect, pathImagen, pathDEF)
{
	SDL_Rect rectReloj;
	rectReloj.w = 10;
	rectReloj.h = 10;
	this->reloj = new RelojArmaSprite(renderer,rectReloj,spriteRelojRojo);
}

void ATiempoDibujable::actualizar(Observable* observable){
	ExplosivaPorTiempo* fig = (ExplosivaPorTiempo*)observable;
	if (fig->getExplotar()) {
		this->destruido = true;
	} else {
		this->rect.x = (fig->getPosicion().x * relacionPPU) - rect.w /2;
		this->rect.y = (fig->getPosicion().y * relacionPPU) - rect.h /2;
		this->anguloRotacion = fig->getBody()->GetAngle() * RADTODEG;
		SDL_Rect rectReloj = this->reloj->getRect();
		rectReloj.x = this->rect.x;
		rectReloj.y = this->rect.y - 5;
		this->reloj->setRect(rectReloj);
		this->reloj->setTiempoRestante(fig->getTiempoRestante());
	}
}

void ATiempoDibujable::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom,int anchoPx, int altoPx){
	ArmaDibujable::dibujar(renderer,corrimientoX,corrimientoY,escalaZoom,anchoPx,altoPx);
	this->reloj->dibujar(renderer,corrimientoX,corrimientoY,escalaZoom,anchoPx,altoPx);
}

ATiempoDibujable::~ATiempoDibujable(void)
{
	//printf("entro antes de reloj");
	delete this->reloj;
}


string ATiempoDibujable::serializar(){
	string serializado = "";
	serializado = StringUtil::int2string(serializadoATiempoDibujable);
	serializado += separadorCampoTipoEntidades;    
	serializado += StringUtil::float2string(this->anguloRotacion);
	serializado += separadorCamposEntidades;    
	serializado += StringUtil::int2string(this->rect.x);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->rect.y);
    serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->rect.w);
	serializado += separadorCamposEntidades;
    serializado += StringUtil::int2string(this->rect.h);
	serializado += separadorCamposEntidades;
	serializado += this->rutaImagen;
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->destruido);

	//Reloj
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->reloj->frame);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->reloj->numCuadros);
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->reloj->tiempoActual);

	return serializado;
}

void ATiempoDibujable::deserealizar(string aDeserealizar){
	vector<string> des = StringUtil::split(aDeserealizar,separadorCampoTipoEntidades);
	//des.at(0) tiene el tipo, des.at(0) tiene el resto de los atributos
	vector<string> atributos = StringUtil::split(des.at(1),separadorCamposEntidades);

	this->anguloRotacion = StringUtil::str2float(atributos.at(0).c_str());
    SDL_Rect rectAux;
	rectAux.x = StringUtil::str2int(atributos.at(1));
	rectAux.y = StringUtil::str2int(atributos.at(2));
	rectAux.w = StringUtil::str2int(atributos.at(3));
	rectAux.h = StringUtil::str2int(atributos.at(4));
	this->setRect(rectAux);

	this->rutaImagen = atributos.at(5);
	this->destruido = StringUtil::str2int(atributos.at(6));

	//Reloj
	this->reloj->frame = StringUtil::str2int(atributos.at(7));
	this->reloj->numCuadros = StringUtil::str2int(atributos.at(8));
	this->reloj->tiempoActual = StringUtil::str2int(atributos.at(9));


} 
#include "RectanguloDibujable.h"


RectanguloDibujable::RectanguloDibujable(float ancho,float alto)
{
	this->n = 4;
	this->posicionesX = new short int[this->n];
	this->posicionesY = new short int[this->n];
	this->ancho = ancho;
	this->alto = alto;
};

RectanguloDibujable::RectanguloDibujable()
{
}

RectanguloDibujable::~RectanguloDibujable()
{
	delete[] this->posicionesX;
	delete[] this->posicionesY;
}



void RectanguloDibujable::actualizar(Observable* observable) {
	Figura* fig = (Figura*)observable;
	b2PolygonShape* shape = (b2PolygonShape*)fig->getBody()->GetFixtureList()->GetShape();
	float escalaX = relacionPPU;
	float escalaY = relacionPPU;
	b2Vec2 posicion = b2Vec2(fig->getPosicion().x * escalaX, fig->getPosicion().y * escalaY);
	float hipotenusa = shape->GetVertex(0).Length();
	for (int i = 0; i < this->n; i++) {
		b2Vec2 vertice = shape->GetVertex(i);
		float anguloNuevo = atan2f(vertice.y,vertice.x) + fig->getAngulo();
		this->posicionesX[i] = hipotenusa * escalaX * coseno[(int) (anguloNuevo * RADTODEG)] + posicion.x;//os(anguloNuevo) + posicion.x;
		this->posicionesY[i] = hipotenusa * escalaY * seno[(int) (anguloNuevo * RADTODEG)] + posicion.y;//in(anguloNuevo) + posicion.y;
	}
}

void RectanguloDibujable::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int anchoPx, int altoPx){
	short int posX[4];
	short int posY[4];
	for (int i=0; i<4;i++) {
		posX[i] = (this->posicionesX[i] * escalaZoom) - corrimientoX;
		posY[i] = (this->posicionesY[i] * escalaZoom) - corrimientoY;
	}
	filledPolygonRGBA(renderer,posX,posY,this->n,this->getColor()[0],this->getColor()[1],this->getColor()[2],255);
}

string RectanguloDibujable::serializar(){
	string serializado = "";
	serializado = StringUtil::int2string(serializadoRectanguloDibujable);
	serializado += separadorCampoTipoEntidades;    
	serializado += StringUtil::float2string(this->ancho);
	serializado += separadorCamposEntidades;    
	serializado += StringUtil::float2string(this->alto);;
	serializado += separadorCamposEntidades;
	serializado += StringUtil::int2string(this->n);
    serializado += separadorCamposEntidades;
	for (int i = 0; i < this->n-1; i++) {
		serializado += StringUtil::int2string(this->posicionesX[i]);
		serializado += separadorCamposArreglo;
	}
	serializado += StringUtil::int2string(this->posicionesX[n-1]);
	serializado += separadorCamposEntidades;

	for (int i = 0; i < this->n-1; i++) {
		serializado += StringUtil::int2string(this->posicionesY[i]);
		serializado += separadorCamposArreglo;
	}
	serializado += StringUtil::int2string(this->posicionesY[n-1]);

	for (int i = 0; i < 3; i++) {
		serializado += separadorCamposEntidades;
		serializado += StringUtil::int2string(this->color[i]);
	}
	return serializado;
}

int RectanguloDibujable::deserealizar(string aDeserealizar){
	vector<string> des = StringUtil::split(aDeserealizar,separadorCampoTipoEntidades);
	//des.at(0) tiene el tipo, des.at(0) tiene el resto de los atributos
	vector<string> atributos = StringUtil::split(des.at(1),separadorCamposEntidades);
	this->ancho = StringUtil::str2float(atributos.at(0).c_str());
    this->alto = StringUtil::str2float(atributos.at(1).c_str());
	this->n = StringUtil::str2int(atributos.at(2));
	
	this->posicionesX = new short int[n];
	this->posicionesY = new short int[n];

	vector<string> posicionesX = StringUtil::split(atributos.at(3),separadorCamposArreglo);
	for (int i = 0; i < this->n; i++) {
		this->posicionesX[i] = StringUtil::str2int(posicionesX.at(i));
	}

	vector<string> posicionesY = StringUtil::split(atributos.at(4),separadorCamposArreglo);
	for (int i = 0; i < this->n; i++) {
		this->posicionesY[i] = StringUtil::str2int(posicionesY.at(i));
	}

	this->color[0] = StringUtil::str2int(atributos.at(5));
	this->color[1] = StringUtil::str2int(atributos.at(6));
	this->color[2] = StringUtil::str2int(atributos.at(7));
	return 0;
}
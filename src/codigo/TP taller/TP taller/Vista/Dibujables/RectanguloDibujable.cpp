#include "RectanguloDibujable.h"


RectanguloDibujable::RectanguloDibujable(float ancho,float alto)
{
	this->n = 4;
	this->posicionesX = new short int[this->n];
	this->posicionesY = new short int[this->n];
	this->ancho = ancho;
	this->alto = alto;
};

RectanguloDibujable::~RectanguloDibujable()
{
	delete[] this->posicionesX;
	delete[] this->posicionesY;
}



void RectanguloDibujable::actualizar(Observable* observable) {
	Figura* fig = (Figura*)observable;
	b2PolygonShape* shape = (b2PolygonShape*)fig->getBody()->GetFixtureList()->GetShape();
	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	float escalaAncho = e->anchoPx / e->anchoU;
	float escalaAlto = e->altoPx / e->altoU;
	b2Vec2 posicion = b2Vec2(fig->getPosicion().x * escalaAncho, fig->getPosicion().y * escalaAlto);
	float hipotenusa = sqrt(this->ancho/2 * this->ancho/2 + this->alto/2 * this->alto/2);
	for (int i = 0; i < this->n; i++) {
		b2Vec2 vertice = shape->GetVertex(i);
		float anguloNuevo = atan2(vertice.y*1.0,vertice.x*1.0) + fig->getAngulo();
		this->posicionesX[i] = hipotenusa * cos(anguloNuevo) + posicion.x;
		this->posicionesY[i] = hipotenusa * sin(anguloNuevo) + posicion.y;
		//cout<<i<<" "<<vertice.x<< " " << vertice.y<<endl;
	}
}

void RectanguloDibujable::dibujar(SDL_Renderer* renderer){
	filledPolygonRGBA(renderer,this->posicionesX,this->posicionesY,this->n,this->getColor()[0],this->getColor()[1],this->getColor()[2],255);
}
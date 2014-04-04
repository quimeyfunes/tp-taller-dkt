#include "RectanguloDibujable.h"


RectanguloDibujable::RectanguloDibujable(float ancho,float alto)
{
	this->n = 4;
	this->posicionesX = new short int[this->n];
	this->posicionesY = new short int[this->n];
	this->ancho = ancho;
	this->alto = alto;

	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();
	escalaX = e->anchoPx / e->anchoU;
	escalaY = e->altoPx / e->altoU;
};

RectanguloDibujable::~RectanguloDibujable()
{
	delete[] this->posicionesX;
	delete[] this->posicionesY;
}



void RectanguloDibujable::actualizar(Observable* observable) {
	Figura* fig = (Figura*)observable;
	b2PolygonShape* shape = (b2PolygonShape*)fig->getBody()->GetFixtureList()->GetShape();

	b2Vec2 posicion = b2Vec2(fig->getPosicion().x * escalaX, fig->getPosicion().y * escalaY);
	float hipotenusa = shape->GetVertex(0).Length();//sqrt(this->ancho/2 * this->ancho/2 + this->alto/2 * this->alto/2);
	for (int i = 0; i < this->n; i++) {
		b2Vec2 vertice = shape->GetVertex(i);
		float anguloNuevo = atan2f(vertice.y,vertice.x) + fig->getAngulo();
		this->posicionesX[i] = hipotenusa * escalaX * cos(anguloNuevo) + posicion.x;
		this->posicionesY[i] = hipotenusa * escalaY * sin(anguloNuevo) + posicion.y;
	}
}

void RectanguloDibujable::dibujar(SDL_Renderer* renderer){
	filledPolygonRGBA(renderer,this->posicionesX,this->posicionesY,this->n,this->getColor()[0],this->getColor()[1],this->getColor()[2],255);
}
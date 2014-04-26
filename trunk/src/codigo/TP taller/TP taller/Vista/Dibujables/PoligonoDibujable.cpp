#include "PoligonoDibujable.h"


PoligonoDibujable::PoligonoDibujable(short int n, float escalaAncho,float escalaAlto)
{
	this->posicionesX = new short int[n];
	this->posicionesY = new short int[n];
	this->n = n;
	this->escalaAncho = escalaAncho;
	this->escalaAlto = escalaAlto;
};

PoligonoDibujable::~PoligonoDibujable()
{
	delete[] this->posicionesX;
	delete[] this->posicionesY;
}



void PoligonoDibujable::actualizar(Observable* observable) {
	Figura* fig = (Figura*)observable;
	b2PolygonShape* shape = (b2PolygonShape*)fig->getBody()->GetFixtureList()->GetShape();
	float escalaAncho = Escenario::getRelacionAncho();
	float escalaAlto = Escenario::getRelacionAlto();
	b2Vec2 posicion = b2Vec2(fig->getPosicion().x * escalaAncho, fig->getPosicion().y * escalaAlto);
	for (int i = 0; i < this->n; i++) {
		b2Vec2 vertice = shape->GetVertex(i);
		float anguloNuevo = atan2(vertice.y*1.0,vertice.x*1.0) + fig->getAngulo();
		this->posicionesX[i] = this->escalaAncho * cos(anguloNuevo) + posicion.x;
		this->posicionesY[i] = this->escalaAlto * sin(anguloNuevo) + posicion.y;
	}
}

void PoligonoDibujable::dibujar(SDL_Renderer* renderer, int corrimientoX,int corrimientoY, float escalaZoom, int posZoomX, int posZoomY){
	filledPolygonRGBA(renderer,this->posicionesX,this->posicionesY,this->n,this->getColor()[0],this->getColor()[1],this->getColor()[2],255);
}

string PoligonoDibujable::serializar(){
	string serializado = "";
	return serializado;
}

void PoligonoDibujable::deserealizar(string aDeserealizar){
}
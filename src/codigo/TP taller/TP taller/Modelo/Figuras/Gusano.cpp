#include "Gusano.h"



Gusano::Gusano()
{
}


Gusano::Gusano(float x, float y, short int rotacion, b2World* world, bool estatico, float ancho, float alto, float masa, int maximosCLientes)
	: Figura(x , y , rotacion, world, estatico, true)
{
	this->armaActual.potenciaDisparo = 0;
	this->armaActual.anguloDisparo=0;
	this->armaActual.armaSeleccionada = NULL;
	this->armaActual.armaTipo = NINGUNA;
	this->armaActual.sentidoDisparo = true;	//true = izq
	this->armaActual.puedeCargarse = false;
	ahogado=false;
	this->congelado = false;
	this->meMuevo=false;
	b2PolygonShape rectanguloShape;
	//Divido a la mitad el ancho y alto para que las medidas sean correctas
	rectanguloShape.SetAsBox(ancho/2, alto/2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &rectanguloShape;
	float areaRec = ancho * alto;
	fixtureDef.density = masa/areaRec;
	fixtureDef.restitution = restitucion;
	fixtureDef.friction = friccion;
	this->fixtureCuerpo = this->getBody()->CreateFixture(&fixtureDef);
	rectanguloShape.SetAsBox(ancho/2,alto/8,b2Vec2(0,alto/2),0);
	b2FixtureDef fixtureDefSensor;
	fixtureDefSensor.isSensor = true;
	fixtureDefSensor.shape = &rectanguloShape;
	int data = GUSANO;
	fixtureDefSensor.userData = this;
	this->getBody()->CreateFixture(&fixtureDefSensor);
	this->numContactos = 0;
	this->maximosCLientes = maximosCLientes;

	//this->meClick = false;
	for(int i=0; i < this->maximosCLientes; i++){
		this->meClickearon.push_back(false);
	}
	this->congelado = false;
	this->explota = false;
	this->activo = false;
	this->vida = vidaGusano;
	this->bufferVida = 0;
	//this->armaSeleccionada = new Bazooka(x - (ancho/2), y, rotacion, world, estatico, ancho /4 ,alto /4, masa /2 );
}

Gusano::~Gusano(void)
{
	//delete this->armaSeleccionada;
}

bool Gusano::meClickeo(float x,float y) {
	for (b2Fixture* f = this->getBody()->GetFixtureList(); f; f = f->GetNext())
	{
		if ((f->GetShape()->TestPoint(this->getBody()->GetTransform(),b2Vec2(x,y))) && !(this->estaMuerto())) {
			  return true;
		  }
	}
	return false;
}

void Gusano::agregarContacto(){
	this->numContactos++;
}

void Gusano::sacarContacto(){
	this->numContactos--;
}

bool Gusano::puedeSaltar(){
	if ((this->numContactos > 0) && !(this->estaMuerto())) return true;
	return false;
}

void Gusano::simularAgua(int nivelAgua){

	if (this->getPosicion().y - altoGusano / 2 > nivelAgua){
		if(!ahogado){
			Reproductor::getReproductor()->detenerSonido(CAMINANDO);
			Reproductor::getReproductor()->reproducirSonido(AGUA);
			ahogado=true;
		}
		b2Vec2 velocidad = this->getBody()->GetLinearVelocity();
		float velocidadY = velocidad.y;
		if (velocidadY > velocidadAgua) {
			if (velocidadY * desaceleracionAgua < velocidadAgua)  {
				velocidadY = velocidadAgua;
			} else {
				velocidadY = velocidadY * desaceleracionAgua;
			}
		}
		this->getBody()->SetLinearVelocity(b2Vec2(0,velocidadY));
		this->setMuerto(true);
		this->setVida(0);
		this->setAhogado(true);
		//this->getBody()->SetType(b2_staticBody);
	}
}

void Gusano::reiniciar(){
	Figura::reiniciar();
	this->getBody()->SetType(b2_dynamicBody);
	this->setMuerto(false);
	this->vida = vidaGusano;
}

bool Gusano::getActivo() {
	return this->activo;
}

void Gusano::setMeClickearon(bool meClickearon, int cliente){
	this->meClickearon[cliente] = meClickearon;
}

bool Gusano::getCongelado() {
	return this->congelado;
}

void Gusano::setCongelado(bool congelado){
	this->congelado = congelado;
}

void Gusano::setActivo(bool activo){
	this->activo = activo;
	if (!activo){
		b2Filter filter = this->fixtureCuerpo->GetFilterData();
		filter.categoryBits = 0x0001; 
		filter.maskBits = 0xFFFF;
		this->fixtureCuerpo->SetFilterData(filter);
	}
}

Arma* Gusano::getArmaSeleccionada(){
	return this->armaActual.armaSeleccionada;
}

void Gusano::setArma(Arma* nueva){
	/*if(this->armaActual.armaSeleccionada != NULL) 
		delete this->armaActual.armaSeleccionada;*/
	this->armaActual.armaSeleccionada = nueva;
	if (nueva != NULL) {
		this->armaActual.armaTipo = nueva->armaTipo;
	} 
}

bool Gusano::tieneUnArma(){
	return (this->armaActual.armaTipo == NINGUNA)? false:true;
}

tipoArma Gusano::getTipoArma(){
	return this->armaActual.armaTipo;
}

void Gusano::setTipoArma(tipoArma tipo){
	this->armaActual.armaTipo = tipo;
}


void Gusano::BeginContact() {
	this->agregarContacto();
}

void Gusano::EndContact(){
	this->sacarContacto();
}

void Gusano::PostSolve(float impulso){
	if (impulso > impulsoCaida) {
		if((this->vida > 0) && (!this->ahogado))
			Reproductor::getReproductor()->reproducirSonido(OUCH);

		this->bufferVida += impulso / 30;
	}
	//if (this->vida < 0){
	//	this->vida = 0;
	//	this->setMuerto(true);
	//}
}

void Gusano::explotar(float fuerza) {
	this->bufferVida += fuerza/200;
	if((this->vida > 0) && (!this->ahogado))
		Reproductor::getReproductor()->reproducirSonido(OUCH);
	//if (this->vida < 0){
	//	this->vida = 0;
	//	this->setMuerto(true);
	//}
}

int Gusano::getVida() {
	return this->vida;
}

void Gusano::disparar() {
	this->tiempoInicial = time(NULL);
	if (this->getArmaSeleccionada()->getPosicion() == this->getPosicion()) {
		this->choqueConArma = false;
		b2Fixture* fixture = this->getArmaSeleccionada()->getBody()->GetFixtureList();
		b2Filter filter = fixture->GetFilterData();

		filter.categoryBits = PROYECTIL;
		filter.maskBits = NORMAL | PROYECTIL;

		fixture->SetFilterData(filter);

		filter = this->fixtureCuerpo->GetFilterData();
		filter.categoryBits = GUSANO;
		filter.maskBits = NORMAL | GUSANO;
	
		for (b2Fixture* fixture = this->getBody()->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
			fixture->SetFilterData(filter);
		}

	} else {
		this->choqueConArma = true;
	}
	this->getArmaSeleccionada()->disparar(this->armaActual.sentidoDisparo, this->armaActual.potenciaDisparo, this->armaActual.anguloDisparo); 
}

void Gusano::chequearChoqueConArma() {
	int tiempoActual = time(NULL);
	if (!this->choqueConArma && this->armaActual.armaSeleccionada != NULL) {
		if (tiempoActual - this->tiempoInicial > 0.5 ) {
			this->tiempoInicial = tiempoActual;
			Arma* arma = this->armaActual.armaSeleccionada;
			b2Fixture* fix = arma->getBody()->GetFixtureList();
			int contador = 0;
			for (b2Fixture* fixture = this->getBody()->GetFixtureList(); fixture; fixture = fixture->GetNext())  {
				if (!b2TestOverlap(fixture->GetShape(),0,fix->GetShape(),0,this->getBody()->GetTransform(),arma->getBody()->GetTransform())) {
					contador++;
				}
			}
			if (contador == 2) {
					this->choqueConArma = true;
					b2Filter filter = fix->GetFilterData();
					filter.maskBits = 0xFFFF;
					filter.categoryBits =  0x0001;
					fix->SetFilterData(filter);
					filter = this->fixtureCuerpo->GetFilterData();
					filter.categoryBits = 0x0001; 
					filter.maskBits = 0xFFFF;
					this->fixtureCuerpo->SetFilterData(filter);
			}
		}
	}	
}

bool Gusano::getExplota(){
	return this->explota;
}

void Gusano::setExplota(bool explota) {
	this->explota = explota;
}

void Gusano::setVida(int vida){
	this->vida = vida;
}

void Gusano::quedateQuietoWachin(){
	Figura::reiniciar();
}
#include "Turno.h"


Turno::Turno(void){
	terminado = true;
	tiempoPorTurno = 10;
	tiempoPorDisparo = 3;
}


Turno::~Turno(void)
{
}


void Turno::reiniciar(){
	tiempoActual = 0;
}

void Turno::detener(){
	detenido = true;
}

void Turno::avanzarTiempo(){
	tiempoActual++;
}

void Turno::setJugadorActual(string jugador){
	jugadorActual = jugador;
}

string Turno::getJugadorActual(){
	return jugadorActual;
}

bool Turno::estaDetenido(){
	return detenido;
}

bool Turno::estaTerminado(){
	return terminado;
}

void Turno::terminar(){
	terminado = true;
	detener();
	reiniciar();
	cout << "TERMINEEEEEE" << endl;
	
}

void Turno::actualizar(){
	if(!detenido){
		int tiempo = time(NULL) - tiempoInicial - tiempoActual;
		if (tiempo != 0){
			if ( tiempoActual >= tiempoPorTurno )	terminar();
			else									tiempoActual++;
		}
	}
	else{
		if(huboDisparo){
			if( (time(NULL) - tiempoInicial - tiempoActual) >= tiempoPorDisparo ) terminar();
		}
	}
}

void Turno::comenzar(){
	detenido = false;
	terminado = false;
	huboDisparo = false;
	tiempoInicial = time(NULL);
	tiempoActual = 0;
	
}

int Turno::getTiempoActual(){
	return tiempoActual;
}

void Turno::esperarDisparo(){
	detener();
	huboDisparo=true;
	
}

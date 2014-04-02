#include "Modelo/Juego/Juego.h"

int main(int argc, char* argv[]){	

	try{
		Juego* juego = new Juego();
		juego->ejecutar();
		delete juego;

	}catch(exception &e){
	
	}
	return 0;    
}
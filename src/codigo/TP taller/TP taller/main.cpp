#include "Modelo/Juego/Juego.h"
#include <process.h>

int main(int argc, char* argv[]){
	if(argc >= 2){
	ParserYaml::setConfigPath(argv[1]);
	}
	else{Logger::getLogger()->escribir("No se recibió ningun parametro.");}
	try{
		Juego* juego = new Juego();
		juego->ejecutar();
		delete juego;

	}catch(exception &e){
		Logger::getLogger()->escribir(e.what());
		Logger::getLogger()->guardarEstado();
	}
	return 0;    
}
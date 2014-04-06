#include "Modelo/Juego/Juego.h"

int main(int argc, char* argv[]){	

	ParserYaml::setConfigPath(argv[1]);
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
#include "Modelo/Red/Cliente.h"
#include <process.h>

Cliente* cliente = NULL;

void clienteLoop() 
{ 
    while(true) 
    {
		cliente->actualizar();
		SDL_Delay(1);
    }
}

int main(int argc, char* argv[]){
	/*if(argc >= 2){
	ParserYaml::setConfigPath(argv[1]);
	}
	else{Logger::getLogger()->escribir("No se recibió ningun parametro.");}*/

	ParserYaml::setConfigPath("config/config.yaml");
	try{
		printf("Ingrese 1 para ser cliente, 2 para ser servidor.\n");
		std::string argumento;
		std::cin >> argumento;
		if(argumento == "1"){
			//Cliente
			printf("Soy Cliente.\n");
			printf("Ingrese su nombre de usuario: ");
			string nombre;
			cin.ignore();
			getline(cin, nombre);

			cliente = new Cliente(nombre);
			clienteLoop();
		}else if(argumento == "2"){
			//Servidor
			printf("Soy Servidor\n");
			Juego* juego = new Juego("a");
			juego->ejecutar();
			delete juego;
		}
	}catch(exception &e){
		Logger::getLogger()->escribir(e.what());
		Logger::getLogger()->guardarEstado();
	}
	return 0;    
}
#include "Modelo/Red/Servidor.h"
#include "Modelo/Red/Cliente.h"
#include "Modelo/Juego/Juego.h"
#include <process.h>

void servidorLoop(void *);
void clienteLoop(void);

Servidor * server;
Cliente * client;

int main(int argc, char* argv[]){	

	// initialize the server
    server = new Servidor();
	// create thread with arbitrary argument for the run function
    _beginthread( servidorLoop, 0, (void*)12);

    // initialize the client 
    client = new Cliente();
	clienteLoop();
	/*if(argc == 2){
	ParserYaml::setConfigPath(argv[1]);
	}
	try{
		Juego* juego = new Juego();
		juego->ejecutar();
		delete juego;

	}catch(exception &e){
		Logger::getLogger()->escribir(e.what());
		Logger::getLogger()->guardarEstado();
	}
	return 0; */   
	return 0;
}

void servidorLoop(void * arg) 
{ 
    while(true) 
    {
        server->actualizar();
    }
}

void clienteLoop()
{
    while(true)
    {
        //do game stuff
       //will later run client->update();
    }
}


#include "Servidor.h"
#include <iostream>
#include <time.h>

unsigned int Servidor::cliente_id; 

Servidor::Servidor(void){
    // id's to assign clients for our table
    cliente_id = 0;
	MAX_CLIENTES = 4;

	this->clientes = new cliente[MAX_CLIENTES];
	for(int i=0; i<MAX_CLIENTES; i++){
		this->clientes[i].activo=false;
		this->clientes[i].time=0;
		this->clientes[i].username= "";
	}
    // set up the server network to listen 
    red = new ServidorRed(); 
}

void Servidor::actualizar() 
{

    // get new clients
	if(cliente_id < MAX_CLIENTES){
		if(red->acceptarNuevoCliente(cliente_id))
		{
			//printf("cliente %d se ha conectado al servidor\n",cliente_id); 
			//aca chequeo si lo acepto o no
			cliente_id++;
		}
	}

	recibirDeClientes();
}


void Servidor::recibirDeClientes()
{
    Paquete* paquete = new Paquete();
    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for(iter = red->sessions.begin(); iter != red->sessions.end(); iter++)
    {
        // get data for that client
        int data_length = red->recibirData(iter->first, network_data);

        if (data_length <= 0) 
        {
            //no data recieved
            //continue;
        }

        int i = 0;
        while (i < data_length) 
        {
			
            paquete->deserializar(&(network_data[i]));
			i+= paquete->getPesoPaquete();

			switch (paquete->getTipo()) {

                case paqueteInicial:
					this->clientes[iter->first].activo=true;
					this->clientes[iter->first].username = paquete->getMensaje();
					this->clientes[iter->first].time = time(NULL);
					cout<<paquete->getMensaje()<<" se ha conectado al servidor."<<endl;
                    break;

                case paqueteEvento:

					printf("El servidor recibio un paquete evento del cliente %i.\n", iter->first);
                    break;

				case paqueteEstado:

					this->clientes[iter->first].time = time(NULL);
					break;

                default:

                    printf("Error en el tipo de paquete.\n");
                    break;
            }
        }
    }

	for(int i=0; i< MAX_CLIENTES; i++){
		if(clientes[i].activo){
			if(time(NULL) - clientes[i].time > 2){	//2 segundos de espera
				clientes[i].activo=false;
				cout<<clientes[i].username<<" se ha desconectado."<<endl;
			}
		}
	}


	delete paquete;
}
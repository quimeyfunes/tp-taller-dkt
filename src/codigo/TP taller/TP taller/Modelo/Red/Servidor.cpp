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

	if(red->acceptarNuevoCliente(cliente_id))
	{
		//dejo que se conecte un 5to cliente, por si estaba congelado, o para poder decirle que no hay mas lugar
		cliente_id++;	
	}
	

	recibirDeClientes();
}

int Servidor::existeUser(string user){

	for(int i=0; i<MAX_CLIENTES; i++){
		if(clientes[i].username == user){
			return i;
		}
	}
	return -1;
}

void Servidor::enviarPaquete(SOCKET sock, int tipoPaquete, string mensaje){

	int largoMensaje = mensaje.size()+1;
	int paquete_tamano = sizeof(int) + sizeof(int) + largoMensaje;
    char* paquete_data = new char[paquete_tamano];

    Paquete* paquete = new Paquete();
    paquete->setTipo(tipoPaquete);
	paquete->setMensaje(mensaje);
	paquete->setTamanio(largoMensaje);
    paquete->serializar(paquete_data);
	Servicio::enviarMensaje(sock, paquete_data, paquete_tamano);
	delete paquete;
	delete paquete_data;
}

void Servidor::recibirDeClientes()
{
    Paquete* paquete = new Paquete();
	int id;
    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for(iter = red->sessions.begin(); iter != red->sessions.end(); iter++)
    {
        // get data for that client
        int data_length = red->recibirData(iter->first, network_data);

        int i = 0;
        while (i < data_length) 
        {
			
            paquete->deserializar(&(network_data[i]));
			i+= paquete->getPesoPaquete();

			switch (paquete->getTipo()) {

                case paqueteInicial:	

					id = existeUser(paquete->getMensaje());
					if(existeUser(paquete->getMensaje()) != -1){	//si existe el username
						if(!clientes[id].activo){					//si ese username esta congelado
							clientes[id].activo=true;				//descongelo y doy bienvenida
							clientes[id].time=time(NULL);
							enviarPaquete(red->sessions.at(iter->first), paqueteInicial, "Bienvenido de nuevo, "+clientes[id].username+ ".");
							cout<<clientes[id].username<<" se ha conectado."<<endl;
						}else{										//si no esta congelado, es xq ya existe un usuario con ese nombre
							enviarPaquete(red->sessions.at(iter->first), paqueteFinal, "Ya existe otro usuario con su nombre.");
						}
					}else{											//si no existe username, tengo que ver si hay lugar para uno nuevo
						if(cliente_id < 4){					    	//si hay lugar 
							this->clientes[iter->first].activo=true;//le asigno un espacio y doy la bienvenida
							this->clientes[iter->first].username = paquete->getMensaje();
							this->clientes[iter->first].time = time(NULL);
							enviarPaquete(red->sessions.at(iter->first), paqueteInicial, "Bienvenido, "+clientes[iter->first].username+".");
							cout<<clientes[iter->first].username<<" se ha conectado."<<endl;
						}else{
																	//si no hay lugar, lo saco
							enviarPaquete(red->sessions.at(iter->first), paqueteFinal, "Ya se ha alcanzado la cantidad maxima de clientes.");
						}
					}
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
#include "Cliente.h" 
#include <iostream>
#include <fstream>

Cliente::Cliente(string nombre, string ip){
    red = new ClienteRed(ip);
	this->username=nombre;
	this->activo = false;
	enviarPaquete(red->socketCliente, paqueteInicial, this->username);
}

void Cliente::enviarPaquete(SOCKET sock, int tipoPaquete, string mensaje){

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

void Cliente::enviarEstado(){

	enviarPaquete(red->socketCliente, paqueteEstado, this->username);
}

void Cliente::enviarEvento(string eventoSerializado){

	enviarPaquete(red->socketCliente, paqueteEvento, eventoSerializado);
}

void Cliente::recibirDeServidor()
{
		Paquete* paquete = new Paquete();
        // get data from server
        int data_length = red->recibirData(network_data);

		
		int tipoPaquete;

        if (data_length <= 0) 
        {
            //no data recieved
			//ver q hacerrrrrrrrrrrrr
        }
		
        int i = 0;
		while (i < data_length) 
        {

			paquete->deserializar(&(network_data[i]));
			tipoPaquete = paquete->getTipo();
			if(tipoPaquete != 1){
				i += paquete->getPesoPaquete();
			}
			else{
				i+= paquete->getTamanio();
			}
			
			ofstream arch;
			int pesoTerreno = paquete->getTamanio() - (2*sizeof(int));
			char* terreno = new char[pesoTerreno];

			switch (tipoPaquete) {

                case paqueteInicial:
					cout<< data_length << endl;
					cout<< "el peso del terreno es: "<< pesoTerreno << endl;
					cout<< "paquete inicial" << endl;
					//recibo [ TIPO | ALTOPX | ANCHOPX | NIVELAGUA | TERRENO ]
					arch.open("terreno.png", std::ofstream::binary);
					arch.seekp(0, ios::beg);
			
					memcpy(terreno, network_data+(2*sizeof(int)), pesoTerreno);
					arch.write(terreno,pesoTerreno);
					arch.close();
					this->activo=true;
					break;

                case paqueteEvento:

					printf("El cliente recibio un paquete evento del servidor.\n");
					break;

				case paqueteVista:
				 	printf("El cliente recibio un paquete vista del servidor.\n");
					this->vistaSerializada = paquete->getMensaje();
					break;

				case paqueteFinal:
					cout<<paquete->getMensaje();
					this->activo=false;
					break;
                default:

                    printf("Error en el tipo de paquete.Tipo es %d\n",paquete->getTipo());

                    break;
            }
        }
		delete paquete;
    }


void Cliente::actualizar() 
{
		recibirDeServidor();
		if(activo)enviarEstado();
}

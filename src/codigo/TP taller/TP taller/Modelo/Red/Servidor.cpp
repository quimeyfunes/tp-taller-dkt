#include "Servidor.h"
#include <iostream>

unsigned int Servidor::cliente_id; 

Servidor::Servidor(void)
{
    // id's to assign clients for our table
    cliente_id = 0;

    // set up the server network to listen 
    red = new ServidorRed(); 
}

void Servidor::actualizar() 
{

    // get new clients
    if(red->acceptarNuevoCliente(cliente_id))
    {
        //printf("cliente %d se ha conectado al servidor\n",cliente_id); 

        cliente_id++;
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

					cout<<paquete->getMensaje()<<" se ha conectado al servidor."<<endl;
					cout<<"Quimey la tenes adentro."<<endl;
                    break;

                case paqueteEvento:

					printf("El servidor recibio un paquete evento del cliente %i.\n", iter->first);
                    break;

                default:

                    printf("Error en el tipo de paquete.\n");
                    break;
            }
        }
    }
	delete paquete;
}
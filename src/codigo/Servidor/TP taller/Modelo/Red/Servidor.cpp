#include "Servidor.h"

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
        printf("cliente %d se ha conectado al servidor\n",cliente_id); 

        cliente_id++;
    }
}

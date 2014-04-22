#include "Cliente.h" 


Cliente::Cliente(void)
{
    red = new ClienteRed();

	 // send init packet
    const unsigned int paquete_tamano = sizeof(Paquete);
    char paquete_data[paquete_tamano];

    Paquete paquete;
    paquete.setTipo(paqueteInicial);

    paquete.serializar(paquete_data);

	Servicio::enviarMensaje(red->socketCliente, paquete_data, paquete_tamano);
}


void Cliente::recibirDeServidor()
{
    Paquete paquete;
   
        // get data from server
        int data_length = red->recibirData(network_data);

        if (data_length <= 0) 
        {
            //no data recieved
			//ver q hacerrrrrrrrrrrrr
        }

        int i = 0;
        while (i < (unsigned int)data_length) 
        {
            paquete.deserializar(&(network_data[i]));
            i += sizeof(Paquete);
			switch (paquete.getTipo()) {

                case paqueteInicial:

                    printf("El cliente recibio el paquete inicial del servidor.\n");

                    break;

                case paqueteEvento:

					printf("El cliente recibio un paquete evento del servidor.\n");

                    break;

                default:

                    printf("Error en el tipo de paquete\n");

                    break;
            }
        }
    }


void Cliente::actualizar() 
{
	recibirDeServidor();
}

#include "Cliente.h" 


Cliente::Cliente(void)
{
    red = new ClienteRed();

	 // send init packet
    const unsigned int paquete_tamano = 500;
    char paquete_data[paquete_tamano];

    Paquete paquete;
    paquete.setTipo(paqueteInicial);

    paquete.serializar(paquete_data);

	Servicio::enviarMensaje(red->socketCliente, paquete_data, paquete_tamano);
}


void Cliente::recibirDeServidor()
{
	printf("Recibir de servidor.\n");
    Paquete paquete;
   
        // get data from server
        int data_length = red->recibirData(network_data);
		list<DibujableSerializado> lista;
		std::vector<std::string> objetosSerializados;
        if (data_length <= 0) 
        {
            //no data recieved
			//ver q hacerrrrrrrrrrrrr
        }

        int i = 0;
		while (i < data_length) 
        {
			printf("ENTRO WHILE.\n");
            paquete.deserializar(&(network_data[i]));
            i += 500;
			switch (paquete.getTipo()) {

                case paqueteInicial:

					printf("El cliente recibio el paquete inicial del servidor.\n");

                    break;

                case paqueteEvento:

					printf("El cliente recibio un paquete evento del servidor.\n");

                    break;
				case paqueteVista:
					printf("El cliente recibio un paquete vista del servidor.\n");
					printf("Paquete tamanio:%d .\n",paquete.getTamanio());
					printf("Pepe2\n");
					//memcpy(&lista, paquete.getMensaje().c_str(), paquete.getTamanio());
					objetosSerializados = StringUtil::split(paquete.getMensaje(),'#');
					printf("Vista deserealizada.\n");
					printf("Tamano de la lista:%d .\n",lista.size());
					for (std::vector<string>::iterator it =objetosSerializados.begin(); it != objetosSerializados.end(); it++) {
						printf("FOR.\n");
						DibujableSerializado dib;
						memcpy(&dib, (*it).c_str(), sizeof(DibujableSerializado));
						printf("PosX:%d .\n",dib.posicionX);
						printf("PosY:%d .\n",dib.posicionY);
					}
					break;
                default:

                    printf("Error en el tipo de paquete.Tipo es %d\n",paquete.getTipo());

                    break;
            }
        }
    }


void Cliente::actualizar() 
{
	recibirDeServidor();
}

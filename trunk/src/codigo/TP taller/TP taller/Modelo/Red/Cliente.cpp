#include "Cliente.h" 

Cliente::Cliente(void)
{
    red = new ClienteRed();

	 // send init packet
    const unsigned int paquete_tamano = sizeof(Paquete);
    char paquete_data[paquete_tamano];

    Paquete paquete;
    paquete.tipoPaquete = INIT_CONNECTION;

    paquete.serializar(paquete_data);

	Servicio::enviarMensaje(red->socketCliente, paquete_data, paquete_tamano);
}
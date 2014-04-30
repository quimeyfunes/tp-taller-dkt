#ifndef __PAQUETE_H__
#define __PAQUETE_H__

#include <string>
#include "../StringUtil.h"
using namespace std;

class Paquete{

private:
	int tipo;
	int tamanio;
	char* mensaje;

public:
	Paquete();
	void serializar(char * data);
    void deserializar(char * data);
	int getTipo();
	void setTipo(int tipo);
	char* getMensajeChar();
	string getMensaje();
	void setMensaje(string msg);
	void setMensajeChar(char* msg);

	int getTamanio();
	void setTamanio(int num);

	int getPesoPaquete();

};
#endif
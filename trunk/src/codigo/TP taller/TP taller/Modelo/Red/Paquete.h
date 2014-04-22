#ifndef __PAQUETE_H__
#define __PAQUETE_H__

#include <string.h>
using namespace std;

class Paquete{

private:
	int tipo;
	char* mensaje;

public:
	Paquete();
	void serializar(char * data);
    void deserializar(char * data);
	int getTipo();
	void setTipo(int tipo);

	char* getMensaje();
	void setMensaje(char* msg);
};
#endif
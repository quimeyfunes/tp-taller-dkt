#ifndef __PAQUETE_H__
#define __PAQUETE_H__

#include <string>
using namespace std;

class Paquete{

private:
	int tipo;
	int tamanio;
	string mensaje;

public:
	Paquete();
	void serializar(char * data);
    void deserializar(char * data);
	int getTipo();
	void setTipo(int tipo);
	string getMensaje();
	void setMensaje(string msg);

	int getTamanio();
	void setTamanio(int num);

	int getPesoPaquete();

};
#endif
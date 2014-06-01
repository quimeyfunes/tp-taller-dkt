#ifndef __TURNO_H__
#define __TURNO_H__

#include <time.h>
#include "../StringUtil.h"

using namespace std;

class Turno
{

private:
	int tiempoInicial;
	int tiempoPorTurno;
	int tiempoPorDisparo;
	string jugadorActual;
	int tiempoActual;
	bool detenido;
	bool terminado;
	bool huboDisparo;

public:
	void setJugadorActual(string jugador);
	string getJugadorActual();
	int getTiempoActual();
	bool estaDetenido();
	bool estaTerminado();
	void reiniciar();
	void esperarDisparo();
	void detener();
	void avanzarTiempo();
	void terminar();
	void actualizar();
	void comenzar();
	Turno(void);
	~Turno(void);
};

#endif
#ifndef __TURNO_H__
#define __TURNO_H__

#include <time.h>
#include "../StringUtil.h"
#include "../../constantes.h"

using namespace std;

class Turno
{

private:
	int tiempoInicial;
	int tiempoDiferido;
	int tiempoDiferidoGolpe;
	int tiempoPorTurno;
	int tiempoPorDisparo;
	int tiempoPorGolpe;
	string jugadorActual;
	int tiempoActual;
	bool detenido;
	bool terminado;
	bool huboDisparo;
	bool huboGolpe;

public:
	void setJugadorActual(string jugador);
	string getJugadorActual();
	int getTiempoActual();
	bool estaDetenido();
	bool estaTerminado();
	void reiniciar();
	void esperarDisparo();
	void esperarGolpe();
	void detener();
	void avanzarTiempo();
	void terminar();
	void actualizar();
	void comenzar();
	Turno(void);
	~Turno(void);
};

#endif
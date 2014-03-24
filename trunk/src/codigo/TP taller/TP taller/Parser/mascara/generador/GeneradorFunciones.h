#pragma once

#include "../../../constantes.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>

class GeneradorFunciones{

public:

	void generarFuncionFourier(double* &funcion, int n, double fMin, double fMax);
	GeneradorFunciones();
	~GeneradorFunciones();

private:

	void generarCoeficientesFourier(double* &vector, int n);
	void obtenerMinMax(double* f, int n, double &min, double &max);

};


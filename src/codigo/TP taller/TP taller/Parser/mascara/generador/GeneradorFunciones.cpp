#include "GeneradorFunciones.h"
#include "../../yaml/ParserYaml.h"

void GeneradorFunciones::generarFuncionFourier(double* &funcion, int n, double fMin, double fMax){

	EscenarioParseado* e = ParserYaml::getParser()->getEscenario();

	double* a = new double[n];
	double* b = new double[n];
	generarCoeficientesFourier(a, n);
	generarCoeficientesFourier(b, n);

	// funcion(x) = sumatoria[a(x).cos(nwx) + b(x).sen(nwx)]
	for (int x = 0; x < n; ++x) { 
		funcion[x] = a[0]; 
		for (int k = 1; k <= n; ++k) funcion[x] += a[k] * cos(2 * PI / n * k * x) + b[k] * sin(2 * PI / n * k * x); 
	}

	//ajusto la funcion a las dimensiones fMin y fMax
	double minF, maxF;
	obtenerMinMax(funcion, n, minF, maxF);
	for(int i=0; i<n; i++) funcion[i] = (funcion[i] - minF) * ((fMax-fMin)/(maxF-minF)) + e->altoPx- fMax;

	//ya no necesito los coeficientes de fourier
	delete[] a;
	delete[] b;
}

void GeneradorFunciones::generarCoeficientesFourier(double* &vector, int n){

	int max =1000;
	srand(time(NULL));
	int num;

	vector[0] = rand()%max;
	//me aseguro que cada valor sea menor al anterior
	for(int i = 1; i<n; i++){

		bool listo = false;
		while(!listo){
			num = rand()%max;
			if(num <= vector[i-1]){
				vector[i] = num;
				listo = true;
			}		
		}
	}
}

void GeneradorFunciones::obtenerMinMax(double *f, int n, double &min, double &max){

	min = f[0];
	max = 0;

	for(int i=0; i<n; i++){
		if(f[i] < min) min = f[i];
		if(f[i] > max) max = f[i];
	}
}

GeneradorFunciones::GeneradorFunciones(){}
GeneradorFunciones::~GeneradorFunciones(){}

#ifndef LECTORTERRENO_H_
#define LECTORTERRENO_H_

#include "../../constantes.h"
#include "../../Logger/Logger.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <vector>

using namespace std;

typedef struct{
	Uint8 R;
	Uint8 G;
	Uint8 B;
}pixel;

typedef struct{
	int x;
	int y;
}punto;

class LectorTerreno{

private:

	Logger* logError;
	bool** matrizTerreno;
	char* rutaMascaraUsada;
	SDL_Surface* imagen;
	int anchoMatriz;
	int altoMatriz;

	bool esBlanco(pixel p);
	bool esNegro(pixel p);
	pixel boolAPixel(bool b);

	/*convierte el vector de RGBA a una matriz de pixeles de dimensiones ancho x alto.
	chequea si el vector es valido y carga una matriz pixel por pixel del terreno a usar.*/
	void RGB_AMatrizBool();
	//mapea una funcion a la matriz
	void cargarFuncionEnMatriz(double* f);
	void guardarMatrizEnPNG(char* nombreArchivo);

	//cuando la imagen de terreno es invalida, se genera una matriz de terreno aleatoria
	void generarMatrizAleatoria();

	/*verifica si en las columnas de la matriz hay un error TIERRA-CIELO-TIERRA*/
	vector<int> chequearTCT(int &numErrores);
	void loguearErroresMatriz(vector<punto> pixelesInvalidos, vector<int> columnasInvalidas);
	void loguearErroresPNG(bool existePNG, bool esPNG);

public:
	
	/*se le pasa por parametro el nombre del archivo, si no existe o no es formato PNG
	se informa en el log y se genera un PNG aleatorio con ese nombre*/
	LectorTerreno(char* nombreArchivo);

	/*genera una imagen PNG de terreno aleatorio valido, con los parametros nombre, alto y ancho (en pixeles)*/
	void generarTerrenoAleatorio(char* nombreArchivo);

	bool** getMatrizTerreno();
	char* getRutaMascaraUsada();
	int getAnchoMatriz();
	int getAltoMatriz(); 

	~LectorTerreno();
};

#endif
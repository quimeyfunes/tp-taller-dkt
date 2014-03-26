#ifndef LECTORTERRENO_H_
#define LECTORTERRENO_H_

#include "../../constantes.h"
#include "../../Logger/Logger.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

using namespace std;

typedef struct{
	Uint8 R;
	Uint8 G;
	Uint8 B;
}pixel;

class lectorTerreno{

private:
	Logger* logError;
	bool** matrizTerreno;
	SDL_Surface* imagen;
	int anchoMatriz;
	int altoMatriz;

	bool esBlanco(pixel p);
	bool esNegro(pixel p);

	/*convierte el vector de RGBA a una matriz de pixeles de dimensiones ancho x alto.
	chequea si el vector es valido y carga una matriz pixel por pixel del terreno a usar.*/
	void RGB_AMatrizBool();
	//mapea una funcion a la matriz
	void cargarFuncionEnMatriz(double* f);
	void guardarMatrizEnPNG(char* nombreArchivo);

	//cuando la imagen de terreno es invalida, se genera una matriz de terreno aleatoria
	void generarMatrizAleatoria();

	/*verifica si en las columnas de la matriz hay un error TIERRA-CIELO-TIERRA*/
	int chequearTCT(bool &error);

public:
	
	/*se le pasa por parametro el nombre del archivo, si no existe o no es formato PNG
	se informa en el log y se genera un PNG aleatorio con ese nombre*/
	lectorTerreno(char* nombreArchivo);

	/*genera una imagen PNG de terreno aleatorio valido, con los parametros nombre, alto y ancho (en pixeles)*/
	void generarTerrenoAleatorio(char* nombreArchivo);

	bool** getMatrizTerreno();
	int getAnchoMatriz();
	int getAltoMatriz(); 

	~lectorTerreno();
};

#endif
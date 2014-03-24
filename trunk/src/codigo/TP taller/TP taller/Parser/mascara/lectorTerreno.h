#ifndef LECTORTERRENO_H_
#define LECTORTERRENO_H_

#include "lodepng.h"
#include "../../constantes.h"
#include "../../Logger/Logger.h"

using namespace std;

typedef struct{
	int R;
	int G;
	int B;
	int A;
}pixel;

class lectorTerreno{

private:
	bool** matrizTerreno;
	int anchoMatriz;
	int altoMatriz;

	bool esBlanco(pixel p);
	bool esNegro(pixel p);
	Logger* logError;

	/*convierte de notacion RGBA a pixel*/
	pixel RGBAaPixel(int r, int g, int b, int a);
	pixel boolAPixel(bool valor);

	/*convierte el vector de RGBA devuelto por libpng a una matriz de pixeles de dimensiones ancho x alto.
	chequea si el vector es valido y devuelve una matriz pixel por pixel del terreno a usar.*/
	void RGBA_AMatrizBool(vector<unsigned char>* imagen);
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
#ifndef __ESCENARIO_H__
#define __ESCENARIO_H__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <valarray>
#include "../constantes.h"

using namespace std;

class Escenario
{
private:
	int altoPx;
    int anchoPx;
    int altoU;
    int anchoU;
    int nivelAgua;
    string imagenTierra;
    string imagenCielo;
public:
	Escenario();
	Escenario(int altoPx,int anchoPx,int altoU,int anchoU,int nivelAgua,string imagenTierra,string imagenCielo);
	int getAltoPx();
    int getAnchoPx();
    int getAltoU();
    int getAnchoU();
    int getNivelAgua();
    string getImagenTierra();
    string getImagenCielo();
};


#endif
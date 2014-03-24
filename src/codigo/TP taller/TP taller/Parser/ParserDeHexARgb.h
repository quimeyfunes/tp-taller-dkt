#include <string>

using namespace std;

/*
SDL usa RGB, y llega hex del yaml. 
Asumo que me llega bien el string: "#XXXXXX" , X de 0 a F
*/
int* parsearDeHexARgb(string hex) {
	string aux;
	int* rgb = new int[3];
	hex.erase(0,1);
	for (int i = 0; i<3; i++) {
		aux.append(hex.begin()+i*2,hex.begin()+(i+1)*2);
		rgb[i] =  strtol(aux.c_str(),NULL,16);
		aux = "";

	}
	return rgb;
}
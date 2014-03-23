#ifndef __YAMLPARSER_H__
#define __YAMLPARSER_H__

#include "yaml-cpp/yaml.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <valarray>
#include "../../constantes.h"

using namespace std;

class ParserYaml
{
private:
	string nombreArchivo;
public:
	ParserYaml(std::string pathArchivo);
	void parsear();

};


#endif

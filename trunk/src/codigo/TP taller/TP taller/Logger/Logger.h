#ifndef LOGGER_H_
#define LOGGER_H_

#include<fstream>
#include<string.h>
#include <time.h>


using namespace std;

class Logger{

private:

	static Logger* logInstancia;
	Logger();
	ofstream* archivo;
	void ponerFecha();
	
public:
	static Logger* getLogger();
	void escribir(string texto);
	~Logger();

};

#endif
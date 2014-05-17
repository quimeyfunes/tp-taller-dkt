#include "../../Modelo/Red/Serializable.h"
#include "../../Vista/Vista.h"

using namespace std; 

class Evento: public Serializable
{

public:
	int accion;
	int x;
	int y;

    Evento(void);
    ~Evento(void);
	virtual string serializar();
	virtual void deserealizar(string aDeserealizar);
private:    
};
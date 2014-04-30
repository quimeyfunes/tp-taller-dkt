#include "Box2D/Box2D.h"
#include "../constantes.h"
#include "Escenario\Escenario.h"



class ResolverContacto : public b2ContactListener
{
private:
	Escenario* escenario;
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);	
public:
	ResolverContacto();
	void setEscenario(Escenario* escenario);
};
#include "Box2D/Box2D.h"
#include "../constantes.h"
#include "Escenario\Escenario.h"

#include <typeinfo>

class ResolverContacto : public b2ContactListener
{
private:
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);
	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
public:
	ResolverContacto();
};
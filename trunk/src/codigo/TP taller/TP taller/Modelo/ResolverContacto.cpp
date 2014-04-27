#include "ResolverContacto.h"


ResolverContacto::ResolverContacto() {
}

void ResolverContacto::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {
	
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();
  
	b2WorldManifold worldManifold;
	contact->GetWorldManifold( &worldManifold );
	  
	// Para ver si gusano toca algo por abajo y volver a saltar
	void* bodyUserData = contact->GetFixtureA()->GetBody()->GetUserData();
	  
	if ( bodyUserData ) {
		Figura* figura = (Figura*) bodyUserData;
		b2Vec2 localNormal = fixtureA->GetBody()->GetLocalVector( worldManifold.normal );
		if (this->escenario->getFiguraActiva() == figura) {
			float angle = b2Atan2( localNormal.y, localNormal.x );
			if ( figura->getTipo() == gusanoTipo && anguloMinimoSalto < angle && angle < anguloMaximoSalto)
				this->escenario->setPuedeSaltar(true);
		}
	}
	  
	bodyUserData = contact->GetFixtureB()->GetBody()->GetUserData();
      
	if ( bodyUserData ) {
		Figura* figura = (Figura*) bodyUserData;
		b2Vec2 localNormal = fixtureB->GetBody()->GetLocalVector( worldManifold.normal );
		if (this->escenario->getFiguraActiva() == figura) {
			float angle = b2Atan2( localNormal.y, localNormal.x );
			if ( figura->getTipo() == gusanoTipo && anguloMinimoSalto < angle && angle < anguloMaximoSalto)
				this->escenario->setPuedeSaltar(true);
		}
	}
}

void ResolverContacto::setEscenario(Escenario* escenario) {
	this->escenario = escenario;
}
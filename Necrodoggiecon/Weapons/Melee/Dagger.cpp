#include "Dagger.h"

Dagger::Dagger()
{
	Debug::Log("Dagger");
	Weapon::SetWeapon("Dagger");
}

Dagger::~Dagger()
{
}


void Dagger::OnFire(Vector3 actorPos, Vector3 attackDir)
{
	Debug::Log("Dagger is firing");
}

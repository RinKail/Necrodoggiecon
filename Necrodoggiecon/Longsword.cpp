#include "Longsword.h"
#include <Necrodoggiecon\Game\AI\CAIController.h>

Longsword::Longsword()
{
	Weapon::SetWeapon("Longsword");
}

Longsword::~Longsword()
{
}

void Longsword::OnFire(Vector3 actorPos, Vector3 attackDir)
{

}

//std::vector<CEntity*> Longsword::GetPlayersInReach(Vector3 actorPos, Vector3 damagePos)
//{
//	std::vector<CAIController*> enemies = Engine::GetEntityOfType<CAIController>();
//
//	if (enemies.size() == 0) //No enemies
//		return nullptr;
//
//	CAIController* closestEnemy = nullptr;
//
//	//Check each enemy
//	for (CAIController* enemy : enemies)
//	{
//
//		if (actorPos.DistanceTo(enemy->GetPosition()) > Weapon::GetRange() && damagePos.DistanceTo(enemy->GetPosition()) > Weapon::GetRange())
//			break;
//
//		if (closestEnemy == nullptr)
//			closestEnemy = enemy;
//		else
//		{
//			if (actorPos.DistanceTo(enemy->GetPosition()) < actorPos.DistanceTo(closestEnemy->GetPosition()))
//				closestEnemy = enemy;
//		}
//	}
//
//	return std::vector<CEntity*>();
//}

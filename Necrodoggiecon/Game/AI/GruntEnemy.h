#pragma once
/*****************************************************************//**
 * \file   GruntEnemy.h
 * \brief  Header file containing all the inherited functions from CAIController and variables needed to control the Melee Enemies.
 * 
 * \author k013044i
 * \date   May 2022
 *********************************************************************/

#include "Cerberus/Core/AI/CAIController.h"

class GruntEnemy :
    public CAIController
{
public:
    GruntEnemy();

    virtual void ChasePlayer(PlayerCharacter* player) override;
    virtual void AttackPlayer(PlayerCharacter* player, float deltaTime) override;

private:
    std::vector<PlayerController*> playersController = Engine::GetEntityOfType<PlayerController>();
};


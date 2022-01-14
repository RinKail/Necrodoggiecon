#include "CTile.h"
#include "CSpriteComponent.h"


CTile::CTile()
{
	sprite = AddComponent<CSpriteComponent>();
	position = Vector3(0, 0, 0);
	scale = Vector3(0, 0, 0);
	rotation = 0.0f;
}

CTile::CTile(int ID, Vector3 Position)
{
	tileID = ID;
	SetPosition(Position);

	sprite = AddComponent<CSpriteComponent>();
	
	
}

void CTile::Update(float deltaTime)
{
}

CTile::~CTile()
{
}

void CTile::ChangeTileID(int TileID)
{
	tileID = TileID;
	if (TileID == 0)
	{
		sprite->LoadTexture("Resources\\Tiles\\Unwalkable.dds");
		
	}
	else
	{
		sprite->LoadTexture("Resources\\Tiles\\Walkable.dds");
		
	}
}

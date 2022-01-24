#include "CGridCursor.h"
#include "CSpriteComponent.h"
#include "CWorld_Edit.h"
#include "Engine.h"
#include "CCamera.h"
#include <DirectXMath.h>

CGridCursor::CGridCursor()
{
	Offset = Vector3(0, 0, 0);

	ActiveCellSprite = AddComponent<CSpriteComponent>();
	ActiveCellSprite->LoadTexture("Resources\\Tiles\\ActiveCell.dds");

	ActiveCellSprite->SetRenderRect(XMUINT2(tileScale, tileScale));
	ActiveCellSprite->SetSpriteSize(XMUINT2(tileScale, tileScale));
	ActiveCellSprite->SetTextureOffset(XMFLOAT2(0, 0));
	
	SetScale(2, 2, 2);

	screenMoved = false;

	cellSelected = false;

	selectedCell_1 = Vector3(0, 0, 0);
	selectedCell_2 = Vector3(0, 0, 0);

	//ActiveCellSprite->SetSpriteSize(XMUINT2(tileScale * 50, tileScale * 50));

	CachedX = 0; 
	CachedY = 0;
	wasMouseReleased = true;
}

void CGridCursor::Update(float deltaTime)
{
	

	Vector3 camPos = Vector3(Engine::camera.GetCameraPosition().x, Engine::camera.GetCameraPosition().y, -10);

	Vector3 MousePos2 = Vector3(Inputs::InputManager::mousePos.x - Engine::windowWidth * 0.5f, -Inputs::InputManager::mousePos.y + Engine::windowHeight * 0.5f, -100);

	MousePos2 /= Engine::camera.GetZoom();



	Vector3 Result = MousePos2 + camPos;
	




	int X = Result.x / (tileScale * tileScaleMultiplier);
	int Y = Result.y / (tileScale * tileScaleMultiplier);
	float scale = (tileScale);


	if (X <= 0) X = 0;
	if (Y <= 0) Y = 0;


	if (X >= mapScale) X = mapScale;
	if (Y <= -mapScale) Y = -mapScale;




	Vector3 Pos = Vector3(X * (64), Y * (64), -5);
	Vector3 PreScale = Vector3(X, Y, -5);
	Vector3 UpScale = (PreScale - selectedCell_1);

	Vector2 RenderScale;


	SetPosition(Pos);

	//ActiveCellSprite->SetSpriteSize(XMUINT2(tileScale * mapScale, tileScale * mapScale));
	if (cellSelected)
	{
		UpScale = (PreScale - selectedCell_1);

		
		
	}
	


	
	if (Inputs::InputManager::IsMouseButtonPressed(Inputs::InputManager::LButton) && wasMouseReleased)
	{
		if (!cellSelected)
		{
			selectedCell_1 = PreScale;
			cellSelected = true;
			wasMouseReleased = false;
		}
		else
		{
			CWorld_Editable::PerformOperation(Vector2(PreScale.x, PreScale.y), Vector2(selectedCell_1.x, selectedCell_1.y));
			selectedCell_1 = Vector3(0, 0, 0);
			cellSelected = false;
		}

		
	}
	else if (!wasMouseReleased && !Inputs::InputManager::IsMouseButtonPressed(Inputs::InputManager::LButton))
	{
		wasMouseReleased = true;
	}

	if (Inputs::InputManager::IsMouseButtonPressed(Inputs::InputManager::RButton))
	{
		if (cellSelected)
		{
			selectedCell_1 = Vector3(0,0,0);
			cellSelected = false;
		}
		


	}
	

	
	//mouseCalc. * -1;


	int X1 = tileScale * UpScale.x;
	int Y2 = tileScale * UpScale.y;


	//ActiveCellSprite->SetSpriteSize(XMUINT2(X1, Y2));
	
	
	

	
}

void CGridCursor::UpdateSize(int X, int Y)
{
	if (X != CachedX || Y != CachedY)
	{
		CachedX = X;
		CachedY = Y;
		ActiveCellSprite->SetRenderRect(XMUINT2(tileScale, tileScale));
		ActiveCellSprite->SetSpriteSize(XMUINT2(tileScale * X, tileScale * Y));
		ActiveCellSprite->SetTextureOffset(XMFLOAT2(0, 0));
	}
	
}

Vector2 CGridCursor::ScreenToWorld(Vector2 Pos)
{

	


	

	float width = Engine::windowWidth;
	float height = Engine::windowHeight;

	float u = Pos.x / width;
	float v = (height - Pos.y) / height;

	float ratio = width / height;
	Vector2 extent = Vector2(25.0f, 25.0f);
	extent *= Engine::camera.GetZoom();

	Vector2 lower = Vector2(Engine::camera.GetCameraPosition().x, Engine::camera.GetCameraPosition().y) - extent;
	Vector2 upper = Vector2(Engine::camera.GetCameraPosition().x, Engine::camera.GetCameraPosition().y) + extent;

	Vector2 pw = Vector2(((1.0f - u) * lower.x + u * upper.x), ((1.0 - v) * lower.y, +v * upper.y));
	
	
	return pw;
	
}

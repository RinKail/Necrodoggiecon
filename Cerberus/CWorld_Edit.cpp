#include "CWorld_Edit.h"
#include "Libraries/json.hpp"
#include <iostream>
#include <fstream>



CellData CWorld_Editable::tileData[mapScale * mapScale];
EditOperationMode CWorld_Editable::operationType = EditOperationMode::None;
Vector2 CWorld_Editable::editOrigin = Vector2(0, 0);
bool CWorld_Editable::selectedCell = false;
bool CWorld_Editable::isQueueLocked = false;


void CWorld_Editable::SetOperationMode(EditOperationMode mode)
{
	operationType = mode;
	switch (mode)
	{
	case EditOperationMode::Additive:
		Debug::Log("OperationMode: Additive");
		break;
	case EditOperationMode::Subtractive:
		Debug::Log("OperationMode: Subtractive");
		break;
	case EditOperationMode::None:
		Debug::Log("OperationMode: None");
		break;
	}
}

void CWorld_Editable::QueueCell(Vector2 Cell)
{
	if (!isQueueLocked)
	{
		if (!selectedCell)
		{
			editOrigin = Cell;
			selectedCell = true;
		}
		else
		{


			PerformOperation(editOrigin, Cell);

			selectedCell = false;
		}
	}

}

void CWorld_Editable::ClearQueue()
{
	
	editOrigin = Vector2(0, 0);
	selectedCell = false;
}

void CWorld_Editable::PerformOperation(Vector2 A, Vector2 B)
{


	Debug::Log("Called Operation");

	switch (operationType)
	{
	case EditOperationMode::Additive:
		AdditiveBox(A, B);
		

		break;
	case EditOperationMode::Subtractive:
		SubtractiveBox(A,B);
		break;
	case EditOperationMode::None:
		break;

	}
	ClearQueue();

	GenerateTileMap();
}

void CWorld_Editable::PerformOperation_ClearSpace()
{
	ClearSpace();
	GenerateTileMap();
}

void CWorld_Editable::LoadWorld_Edit()
{
	std::ifstream file("Resources/Levels/Level_1.json");


	json storedFile;

	file >> storedFile;

	std::vector<std::string> convertedFile = storedFile["TileData"];


	std::string Test = convertedFile[0];
	std::cout << "" << std::endl;


	for (int i = 0; i < (mapScale * mapScale); i++)
	{
		Vector3 temp = Vector3((float)(i % mapScale), (float)(i / mapScale), 0);
		Vector2 gridPos = Vector2(temp.x, temp.y);

		int ID = atoi(convertedFile[i].c_str());
		Vector3 tempPos = (Vector3(temp.x, temp.y, 0) * (tileScale * 2));

		//tempPos += Vector3(0, 64 * tileScale, 0.0f);

		tempPos.z = 10;



		CTile* Tile = Engine::CreateEntity<CTile>();
		Tile->SetPosition(tempPos);
		Tile->SetScale(tileScaleMultiplier);
		Tile->ChangeTileID(ID);

		tileContainer[i] = Tile;



		if (Tile->GetTileID() != 1)
		{
			tileData[i].id = 0;
		}
		else tileData[i].id = 1;


		//tileData[i].id = Tile->GetTileID();
		tileData[i].type = CellType::Empty;


		

	}

	

	BuildNavigationGrid();

	GenerateTileMap();
}




void CWorld_Editable::SaveWorld(int Slot)
{

	std::ifstream loadedData("Resources/Levels/Level_1.json");


	json SaveData;

	std::vector<std::string> MapData;

	GenerateTileMap();


	for (int i = 0; i < mapScale * mapScale; i++)
	{
		MapData.push_back(std::to_string(tileContainer[i]->GetTileID()));
	}


	SaveData["TileData"] = MapData;


	std::ofstream o("Resources/Levels/Level_1.json");
	o << SaveData;


}

void CWorld_Editable::EditWorld(int Slot)
{
	ClearSpace();

	//AdditiveBox(Vector2(0, 0), Vector2(mapScale, mapScale));

	//SubtractiveBox(Vector2(5, 5), Vector2(mapScale - 5, mapScale - 5));


	//AdditiveBox(Vector2(7, 7), Vector2(10, 10));

	//AdditiveBox(Vector2(15, 15), Vector2(23, 23));

	//GenerateTileMap();

	//SaveWorld(0);
}

void CWorld_Editable::NewWorld(int Slot)
{

	for (int i = 0; i < mapScale * mapScale; i++)
	{
		Vector3 ConvertedPos = IndexToGrid(i);
		

		tileData[i].id = 0;
		tileData[i].type= CellType::Empty;

	}

	for (int i = 0; i < mapScale * mapScale; i++)
	{
		Vector3 convertedPos = IndexToGrid(i);
		Vector3 tempPos = (Vector3(convertedPos.x, convertedPos.y, 0) * (tileScale * 2));
	


		CTile* Tile = Engine::CreateEntity<CTile>();
		Tile->SetPosition(tempPos);
		Tile->SetScale(tileScaleMultiplier);
		Tile->ChangeTileID(0);

		tileContainer[i] = Tile;
	}

}




void CWorld_Editable::ClearSpace()
{
	for (int i = 0; i < mapScale * mapScale; i++)
	{
		tileData[i].id = 0;
	}
}

void CWorld_Editable::AdditiveBox(Vector2 A, Vector2 B)
{

	BoxOperation(A, B, 0);
	

}

void CWorld_Editable::SubtractiveBox(Vector2 A, Vector2 B)
{
	BoxOperation(A, B, 1);
	
}

void CWorld_Editable::AdditiveBox_Scale(Vector2 A, Vector2 B)
{

	BoxOperation(A, B + A, 0);
	
}

void CWorld_Editable::SubtractiveBox_Scale(Vector2 A, Vector2 B)
{
	BoxOperation(A, A + B, 1);
	
}

void CWorld_Editable::BoxOperation(Vector2 A, Vector2 B, int TileID)
{
	Vector2 dimensions = B - A;

	int yMultiplier = 1;
	int xMultiplier = 1;

	if (dimensions.x < 0)
	{
		xMultiplier = -1;
		
	}
	if (dimensions.y < 0)
	{
		yMultiplier = -1;
	
	}


	Debug::Log("Additive Operation: [%f | %f]", dimensions.x, dimensions.y);

	for (int x = 0; x != dimensions.x; x+=xMultiplier)
	{
		for (int y = 0; y != dimensions.y; y+=yMultiplier)
		{
			Vector3 Pos = Vector3((float)x, (float)y, 0);



			int Index = (x + (int)A.x) + ((y + (int)A.y) * mapScale);
			
			if (A.x + x > 0 && A.x + x < mapScale && A.y + y > 0 && A.y + y < mapScale)
				tileData[Index].id = TileID;


		}
	}

}

void CWorld_Editable::RefreshTileMapRegion(Vector2 A, Vector2 B)
{

	Vector2 dimensions = (B + Vector2(1, -1)) - (A + Vector2(-1, 1));

	for (int x = A.x - 1; x < dimensions.x; x++)
	{
		for (int y = A.y + 1; y < dimensions.y; y++)
		{

			int i = GridToIndex(Vector2(x, y));
			Vector2 pos = Vector2(x, y);

			if (A.x + x > 0 && A.x + x < mapScale && A.y + y > 0 && A.y + y < mapScale)
			{
				if (tileData[i].id == 0)
				{
					

					if (IsFloorAdjacent(Vector2(pos.x, pos.y)))
					{
						tileData[i].type = CellType::Edge;
					}
					else tileData[i].type = CellType::Empty;

				}
				else if (tileData[i].id == 1)
				{
					tileData[i].type = CellType::Floor;
				}
			}


		}
	}


}

void CWorld_Editable::GenerateTileMap()
{
	

	for (int i = 0; i < mapScale * mapScale; i++)
	{
		if (tileData[i].id == 0)
		{
			Vector2 pos = Vector2((float)(i % mapScale), (float)(i / mapScale));

			if (IsFloorAdjacent(Vector2(pos.x, pos.y)))
			{
				tileData[i].type = CellType::Edge;
			}
			else tileData[i].type = CellType::Empty;

		}
		else if (tileData[i].id == 1)
		{
			tileData[i].type = CellType::Floor;
		}

	}
	

	for (int i = 0; i < mapScale * mapScale; i++)
	{

		Vector3 pos = IndexToGrid(i);
		
		SetCorner(Vector2(pos.x, pos.y));

	}
	

	for (int i = 0; i < mapScale * mapScale; i++)
	{
		Vector3 temp = IndexToGrid(i);

		//Vector3 Test = Vector3(1, 9, 0);


		Vector2 Pos = Vector2(temp.x, temp.y);
		Vector2 FloorResult = FindAdjacents(Pos, CellType::Floor);
		Vector2 FloorResultDiagonal = FindFloorAdjacentDiagonal(Pos);
		Vector2 EdgeAdjacentResult = FindAdjacents(Pos, CellType::Edge);

		switch (tileData[i].type)
		{
		case CellType::Edge:


			if (FloorResult == Vector2(0, -1)) tileContainer[i]->ChangeTileID(CellID::W_N);
			else if (FloorResult == Vector2(0, 1)) tileContainer[i]->ChangeTileID(CellID::W_S);
			else if (FloorResult == Vector2(1, 0)) tileContainer[i]->ChangeTileID(CellID::W_W);
			else if (FloorResult == Vector2(-1, 0)) tileContainer[i]->ChangeTileID(CellID::W_E);
			break;
		case CellType::Empty:
			tileContainer[i]->ChangeTileID(CellID::N);
			break;
		case CellType::Floor:
			tileContainer[i]->ChangeTileID(CellID::F);
			break;
		case CellType::InnerCorner:



			if (EdgeAdjacentResult == Vector2(1, -1)) tileContainer[i]->ChangeTileID(CellID::IC_NW);
			if (EdgeAdjacentResult == Vector2(-1, -1)) tileContainer[i]->ChangeTileID(CellID::IC_NE);
			if (EdgeAdjacentResult == Vector2(-1, 1)) tileContainer[i]->ChangeTileID(CellID::IC_SW);
			if (EdgeAdjacentResult == Vector2(1, 1)) tileContainer[i]->ChangeTileID(CellID::IC_SE);


			break;
		case CellType::OuterCorner:

			if (FloorResultDiagonal == Vector2(1, -1)) tileContainer[i]->ChangeTileID(CellID::OC_NW);
			else if (FloorResultDiagonal == Vector2(-1, -1)) tileContainer[i]->ChangeTileID(CellID::OC_NE);
			else if (FloorResultDiagonal == Vector2(-1, 1)) tileContainer[i]->ChangeTileID(CellID::OC_SE);
			else if (FloorResultDiagonal == Vector2(1, 1)) tileContainer[i]->ChangeTileID(CellID::OC_SW);

			break;
		}
	}





}

bool CWorld_Editable::IsFloorAdjacent(Vector2 Position)
{
	if (Position.x > 1 && Position.y > 1 && Position.x < mapScale - 1 && Position.y < mapScale - 1)
	{
		return (tileData[GridToIndex(Position + Vector2(1, 0))].id == 1 ||
			tileData[GridToIndex(Position + Vector2(-1, 0))].id == 1 ||
			tileData[GridToIndex(Position + Vector2(0, 1))].id == 1 ||
			tileData[GridToIndex(Position + Vector2(0, -1))].id == 1 ||

			tileData[GridToIndex(Position + Vector2(1, 1))].id == 1 ||
			tileData[GridToIndex(Position + Vector2(-1, -1))].id == 1 ||
			tileData[GridToIndex(Position + Vector2(1, -1))].id == 1 ||
			tileData[GridToIndex(Position + Vector2(-1, 1))].id == 1);
	}
	else return false;
}

Vector2 CWorld_Editable::FindAdjacents(Vector2 Pos, CellType ID)
{

	int X;
	int Y;
	if (tileData[GridToIndex(Pos + Vector2(1, 0))].type == ID)
	{
		X = 1;
		if (tileData[GridToIndex(Pos + Vector2(-1, 0))].type == ID) X = 2;
	}
	else if (tileData[GridToIndex(Pos + Vector2(-1, 0))].type == ID)
	{
		X = -1;
	}
	else
		X = 0;

	if (tileData[GridToIndex(Pos + Vector2(0, 1))].type == ID)
	{
		Y = 1;
		if (tileData[GridToIndex(Pos + Vector2(1, -1))].type == ID) Y = 2;
	}
	else if (tileData[GridToIndex(Pos + Vector2(0, -1))].type == ID)
	{
		Y = -1;
	}
	else Y = 0;




	return Vector2((float)X, (float)Y);
}

Vector2 CWorld_Editable::FindFloorAdjacentDiagonal(Vector2 Position)
{
	if (IsTile(Position + Vector2(1, 1), CellType::Floor)) return Vector2(1, 1);
	else if (IsTile(Position + Vector2(-1, -1), CellType::Floor)) return Vector2(-1, -1);
	else if (IsTile(Position + Vector2(-1, 1), CellType::Floor)) return Vector2(-1, 1);
	else if (IsTile(Position + Vector2(1, -1), CellType::Floor)) return Vector2(1, -1);
	else return Vector2(0, 0);
}

bool CWorld_Editable::SetCorner(Vector2 Position)
{
	if (IsTile(Position, CellType::Edge))
	{
		if (IsTile(Position + Vector2(1, 0), CellType::Edge) && IsTile(Position + Vector2(0, -1), CellType::Edge))
		{

			if (IsTile(Position + Vector2(1, -1), CellType::Floor))
			{
				tileData[GridToIndex(Position)].type = CellType::OuterCorner;
			}
			else if (IsTile(Position + Vector2(-1, 1), CellType::Floor))
			{
				tileData[GridToIndex(Position)].type = CellType::InnerCorner;
			}
		}
		if (IsTile(Position + Vector2(1, 0), CellType::Edge) && IsTile(Position + Vector2(0, 1), CellType::Edge))
		{

			if (IsTile(Position + Vector2(1, 1), CellType::Floor))
			{
				tileData[GridToIndex(Position)].type = CellType::OuterCorner;
			}
			else if (IsTile(Position + Vector2(-1, -1), CellType::Floor))
			{
				tileData[GridToIndex(Position)].type = CellType::InnerCorner;
			}
		}
		if (IsTile(Position + Vector2(-1, 0), CellType::Edge) && IsTile(Position + Vector2(0, -1), CellType::Edge))
		{

			if (IsTile(Position + Vector2(-1, -1), CellType::Floor))
			{
				tileData[GridToIndex(Position)].type = CellType::OuterCorner;
			}
			else if (IsTile(Position + Vector2(1, 1), CellType::Floor))
			{
				tileData[GridToIndex(Position)].type = CellType::InnerCorner;
			}
		}
		if (IsTile(Position + Vector2(-1, 0), CellType::Edge) && IsTile(Position + Vector2(0, 1), CellType::Edge))
		{

			if (IsTile(Position + Vector2(-1, 1), CellType::Floor))
			{
				tileData[GridToIndex(Position)].type = CellType::OuterCorner;
			}
			else if (IsTile(Position + Vector2(1, -1), CellType::Floor))
			{
				tileData[GridToIndex(Position)].type = CellType::InnerCorner;
			}
		}
	}

	return false;
}

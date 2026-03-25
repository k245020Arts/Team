#include "AStar.h"

AStar::AStar()
{
	CreatePoint();
	time = 0.0f;
}

AStar::~AStar()
{
}

void AStar::Update()
{
	time += Time::DeltaTimeRate();

	if (time >= 1)
	{
		time = 0.0f;
		line.clear();

		for (int i = 0; i < pointPos.size(); i++)
		{
			for (int j = 0; j < 8; j++)
			{

			}
		}
	}
}

void AStar::Draw()
{
	for (auto itr : pointPos)
		DrawSphere3D(itr, 40.0f, 32, GetColor(255, 0, 0), GetColor(255, 255, 255), true);
	/*for (auto itr : line)
		DrawLine3D(itr.posA, itr.posB, GetColor(255, 255, 255));*/
}

void AStar::CreatePoint()
{
	VECTOR3 posZero = VECTOR3(-WALL_EDGE_POS / 2, 50, -WALL_EDGE_POS / 2);

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			pointPos.push_back(posZero + VECTOR3(POINT_POS * j, 0, POINT_POS * i));
		}
	}
}

bool AStar::CheckLine()
{


	return false;
}

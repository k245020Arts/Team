#pragma once
#include "../../Library/GameObject.h"
#include "../Stage/StageManager.h"

class AStar : public GameObject
{
public:
	AStar();
	~AStar();

	void Update()override;
	void Draw()override;

	void CreatePoint();
	bool CheckLine();
private:
	const float POINT_POS = WALL_EDGE_POS / 5;//横に何個ポイントを置くか

	struct Line
	{
		VECTOR3 posA;
		VECTOR3 posB;
		bool isLine;

		Line(VECTOR3 _posA, VECTOR3 _posB, bool _isLine)
		{
			posA = _posA;
			posB = _posB;
			isLine = _isLine;
		}
	};
	std::list<VECTOR3> pointPos;
	std::list< Line> line;
	float time;
};
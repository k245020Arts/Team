#pragma once
#include "Transitor.h"

class PushTransitor : public Transitor
{
public:
	enum Direction
	{
		UP,
		DOWN,
		RIGHT,
		LEFT,
	};
	PushTransitor();
	~PushTransitor();

	void Update()override;
	void Draw()override;

	void StartPush(std::string _sceneName, float _time, Direction _dir);

private:
	Direction dir;
};

#pragma once
#include "Transitor.h"

class WipeTransitor : public Transitor
{
public:
	enum Direction
	{
		UP,
		DOWN,
		RIGHT,
		LEFT,
	};
	WipeTransitor();
	~WipeTransitor();

	void Update()override;
	void Draw()override;

	void WipeStart(std::string _sceneName, float _time, Direction _dir);
	void SetGradation();

private:
	Direction dir;
	int hGradation;
};

#pragma once
#include "../../../Library/GameObject.h"

class Color;

class Fead : public GameObject
{
public:
	Fead();
	~Fead();

	void Update()override;
	void Draw()override;

	void FeadIn(float _time,int _color,std::function<int(int, int, float)> _func);
	void FeadOut(float _time, int _color, std::function<int(int, int, float)> _func);

	bool IsFead() { return feedTime >= 0.0f; };


private:

	float feedTime;
	float feedCountMax;

	int color;
	int alpha;
	std::function<int(int, int, float)> easingFunc;
	bool feadIn;
};
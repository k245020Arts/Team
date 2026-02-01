#pragma once
#include "../../../Library/GameObject.h"
#include "../Easing.h"

class Color;

class Fead : public GameObject
{
public:

	enum FeadState
	{
		NONE = -1,
		FEAD_OUT,
		FEAD_IN,
	};
	Fead();
	~Fead();

	void Update()override;
	void Draw()override;

	void FeadIn(float _time,int _color,std::function<int(int, int, float)> _func);
	void FeadOut(float _time, int _color, std::function<int(int, int, float)> _func);

	bool IsFead() { return feedTime > 0.0f; };

	FeadState GetFeadState() { return feadIn; }


private:

	float feedTime;
	float feedCountMax;

	int color;
	int alpha;
	std::function<int(int, int, float)> easingFunc;
	FeadState feadIn;
};
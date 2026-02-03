#pragma once
#include "../../Library/GameObject.h"

class CutSceneBox : public GameObject
{
public:
	CutSceneBox();
	~CutSceneBox();

	void Update()override;
	void Draw()override;

	void StartBox(float _time, int _color, std::function<int(int, int, float)> _func);
	void FinishBox(float _time, int _color, std::function<int(int, int, float)> _func);

	bool IsFead() { return feedTime > 0.0f; };

private:

	float feedTime;
	float feedCountMax;

	int alpha;
	std::function<int(int, int, float)> easingFunc;

	bool feadIn;

	const int BOX_SIZE = 150;

	const int UP_BOX_LIMIT = BOX_SIZE;
	const int DOWN_BOX_LIMIT = 1080 - BOX_SIZE;

	int boxPosY;
};

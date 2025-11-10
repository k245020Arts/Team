#pragma once
#include "Component.h"

class Anim2D : public Component
{
public:
	Anim2D();
	~Anim2D();

	void Update()override;

	void AnimSetting(float _speed, int num);

	bool IsPlaying() { return playing; }

	float GetAnimCounter() { return animCounter; }
	int GetGraphNum() { return graphNum; }

	void AnimStop() { playing = false; }
	void AnimPlay() { playing = true; }
	void AnimReset() { animCounter = 0.0f; }

private:

	bool playing; //アニメーションが再生中かをとる
	float animCounter; //アニメーションのカウンタ
	int graphNum; //分割の枚数
	float animSpeed; //アニメーションのスピードの倍率
};

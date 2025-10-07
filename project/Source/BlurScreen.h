#pragma once
#include "../Library/GameObject.h"

//ブラースクリーンを生成できるクラス。

class BlurScreen : public GameObject
{
public:
	BlurScreen();
	~BlurScreen();

	void Update()override;
	void Draw()override;

	/// <summary>
	/// ブラースクリーンの開始
	/// </summary>
	/// <param name="_time">発生させる時間</param>
	/// <param name="_fadeTime">フェードアウトの時間</param>
	void Play(float _time,float _fadeTime);

	void ScreenFinish();

	void FeedOut();

	void Reset();

private:

	void CurrentScreenSet();

	static const int SCREEN_NUM = 2;

	bool use;

	float alpha;
	bool fadeOut;

	int blurScreen[SCREEN_NUM];

	float defalutAlpha;

	float time;
	float fadeTime;

	float defalutFadeTime;
	int screenDraw;

	int currentScreen;
};

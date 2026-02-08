#include "EnemyDamageUI.h"
#include "../../Common/InputManager/ControllerInputManager.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../Component/Transform/Transform.h"
#include "../../Component/Color/Color.h"
#include "../ComponentManager.h"
#include "../../Common/Easing.h"
#include "../MeshRenderer2D/MeshRenderer2D.h"

EnemyDamageUI::EnemyDamageUI()
{
	numImage = -1;
	damageNum = 0;
	movePos = VZero;
	moveMaxPos = VZero;
	timer = 0.0f;
	timerMax = 0.0f;

	scale = 0.5f;
	alpha = 255;

	feedInOutTime = 0.0f;
	feedOut = false;
	feedIn = true;

	digitCount = 0;
	waitTimer = 0.06f;

	worldTransform = nullptr;
	worldOffset = VZero;
}

EnemyDamageUI::~EnemyDamageUI()
{
	//DeleteGraph(numImage);
}

void EnemyDamageUI::Update()
{
	float dt = Time::DeltaTimeRate();

	if (!feedOut)
	{
		movePos = Easing::EasingFlow<VECTOR3>(&timer,timerMax,VZero,moveMaxPos,Easing::EaseOut<VECTOR3>);

		float rate = 1.0f - (timer / timerMax);
		scale = rate * 1.0f;

		if (feedIn)
		{
			alpha = Easing::EasingFlow<int>(&feedInOutTime, feedInOutTimeMax, 255, 0, Easing::EaseOut<int>);
			if (feedInOutTime <= 0.0f)
			{
				feedInOutTime = 0.0f;
				feedIn = false;
			}
		}

		if (timer <= 0.0f)
		{
			timer = 0.0f;
			feedOut = true;
			feedInOutTime = 0.25f;
			feedInOutTime = feedInOutTimeMax;
		}
	}
	else
	{
		alpha = Easing::EasingFlow<int>(&feedInOutTime, feedInOutTimeMax, 0, 255, Easing::EaseOut<int>);
		if (feedInOutTime <= 0.0f)
		{
			feedInOutTime = 0.0f;
			obj->DestroyMe();
		}
	}
}

void EnemyDamageUI::Draw()
{
	if (numImage < 0 || worldTransform == nullptr) {
		return;
	}

	VECTOR3 worldPos = worldTransform->position + worldOffset + movePos;

	//World座標をScreen座標に画面変換
	VECTOR3 screenPos = ConvWorldPosToScreenPos(worldPos);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	float reTime = timerMax - timer;

	float startX = screenPos.x - (graphSize.x * scale * digitCount) * 0.5f;

	int value = damageNum;

	for (int i = 0; i < digitCount; i++)
	{
		int digit = value % 10;
		value /= 10;

		//一文字一文字ずつ描画させている。
		float charTime = reTime - waitTimer * i;
		//一個一個ずつ出していて、まだ出番ではないので出さない。
		if (charTime < 0) {
			continue;
		}

		const float CHAR_TIME_MAX = 0.2f;
		float t = charTime / CHAR_TIME_MAX;

		if (t >= CHAR_TIME_MAX) {
			t = 1.0f;
		}

		const float BOUNCE_ADD = 42.0f;
		
		float bounce = sinf(180.0f * DegToRad * t) * BOUNCE_ADD;
		const float MAX_SCALE = 0.05f;
		float charScale = scale + sinf(180.0f * DegToRad * t) * MAX_SCALE;

		DrawRectRotaGraph(startX + (digitCount - 1 - i) * graphSize.x * scale,screenPos.y - bounce,digit * graphSize.x,
			0,graphSize.x,graphSize.y,charScale,0.0f,numImage,true);
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void EnemyDamageUI::SetInformation(const VECTOR3& _offset, int _damageNum, const VECTOR3& _movePos, float _timer, int _image, Transform* _world, const VECTOR2I& _graphsize)
{
	worldTransform = _world;
	worldOffset = _offset;

	damageNum = _damageNum;
	moveMaxPos = _movePos;

	timer = _timer;
	timerMax = _timer;

	numImage = _image;
	graphSize = _graphsize;

	// 桁数計算
	int temp = damageNum;
	digitCount = 0;
	do {
		digitCount++;
		temp /= 10;
	} while (temp > 0);

	scale = 0.4f;
	alpha = 0;
	feedIn = true;
	feedOut = false;
	feedInOutTime = 0.2f;
	feedInOutTimeMax = feedInOutTime;
	obj->SetDrawOrder(-1000);
}

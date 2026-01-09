#pragma once
#include "../EnemyState/EnemyStateBase.h"

class T_EnemyDead :public EnemyStateBase
{
public:
	T_EnemyDead();
	~T_EnemyDead();
	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;
private:
	/// <summary>
	/// ‚«”ò‚Ñ‚Ì‹O“¹
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="a">‹È‚ª‚è‚Ì‹­‚³</param>
	/// <param name="h">ˆê”Ô‚‚­‚È‚éêŠ</param>
	/// <param name="k">‚»‚Ì‚Ì‚‚³</param>
	/// <returns></returns>
	float Orbit(float pos, float a, float h, float k);

	const float SPEED = 20.0f;
	float counter;
};
#pragma once
#include "stateBase.h"
#include "Debug.h"
#include "ID.h"
#include "transform.h"
#include "function.h"

class PlayerStateBase : public StateBase
{
public:
	PlayerStateBase();
	~PlayerStateBase();
	void Update() override;
	void Draw()override;

	virtual void Start()override;

	void AvoidStart()override;
	/// <summary>
	/// UŒ‚‚Ì“–‚½‚è”»’è‚Ì¶¬‚ğ‚ ‚é’ö“x“ˆê‰»‚³‚¹‚½ŠÖ”
	/// </summary>
	void AttackCollsion();

	/// <summary>
	/// •’Ê‚Ìó‘Ô‚Å•à‚­‚Æ‚«‚É‚Í‚±‚ÌŠÖ”‚ğŒÄ‚Ô
	/// </summary>
	void DefalutWalk();

protected:
	ID::IDType animId;
	bool noStateChange;
	bool firstColl;
	Transform collTrans;
	bool nextAttack;
};
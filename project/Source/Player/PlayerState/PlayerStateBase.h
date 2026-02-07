#pragma once
#include "../../State/StateBase.h"
#include "../../Common/Debug/Debug.h"
#include "../../Common/ID/ID.h"
#include "../../Component/Transform/Transform.h"
#include "../../Common/function.h"

class PlayerStateBase : public StateBase
{
public:
	PlayerStateBase();
	virtual~PlayerStateBase();
	virtual void Update() override;
	virtual void Draw()override;

	virtual void Start()override;

	void AvoidStart()override;
	
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
	float blendSpeed;
};
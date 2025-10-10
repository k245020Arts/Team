#pragma once
#include "stateBase.h"

class BossIdol : public StateBase
{
public:
	BossIdol();
	~BossIdol();

	void Update()override;
	void Draw()override;

	void Start()override;
	void Finish()override;

private:

};

#pragma once

class T_EnemyStatus
{
private:
	struct Status
	{
		const float normalAttack1 = 20.0f;	//UŒ‚‚P‚Ìƒ_ƒ[ƒW”’l
		
		const float maxHp = 100;			//Å‘åHP
		const float coolTime = 30.0f;		//ˆê’i–Ú‚ÌUŒ‚‚Ü‚Å‚ÌŠÔ
		const float runSpeed = 5.0f;		//ˆÚ“®‘¬“x
		//float fallFrame = 50.0f;			//Ÿ‚ÌUŒ‚‚ÌŠÔŠu‚ğ’Z‚­‚·‚é
		const float range = 1400.0f;		//idel‚Ærun‚ÌØ‚è‘Ö‚í‚è
		const float atkRang = 500.0f;		//run‚ÆUŒ‚‚ÌØ‚è‘Ö‚í‚è
		const float chaseRange = 5000.0f;	//ƒvƒŒƒCƒ„[‚Æ—£‚ê‚½‚Æ‚«‚ÉØ‚è‘Ö‚í‚é
	};
	Status status;

public:
	T_EnemyStatus() { ; }
	Status GetStatus() { return status; }
};
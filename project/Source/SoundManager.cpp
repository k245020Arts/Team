#include "SoundManager.h"
#include "Sound.h"
#include "LoadManager.h"
#include "Debug.h"

SoundManager::SoundManager()
{
	DontDestroyOnSceneChange(true);
}

SoundManager::~SoundManager()
{
	AllDeleteSound();
}

void SoundManager::Update()
{
}

void SoundManager::Draw()
{
}

void SoundManager::SoundLoad(Sound_ID::SOUND_ID _type, std::string path, std::string exten, int volume)
{
	Sound* soundData = new Sound();
	soundData->SoundData(Load::LoadSound(path, exten, _type), volume);
	sound[Sound_ID::GetSoundID(_type)] = soundData;
}

void SoundManager::PlaySceneLoad()
{
	SoundLoad(Sound_ID::SWORD_HIT_SOUND,"swordHit", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_HIT_SOUND1,"swordHit000001", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_HIT_SOUND2,"swordHit000002", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_HIT_SOUND3,"swordHit000003", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_HIT_SOUND4,"swordHit000004", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_HIT_SOUND5,"swordHit000005", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_HIT_SOUND6,"swordHit000006", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_HIT_SOUND7,"swordHit000007", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_HIT_SOUND8,"swordHit0008", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_HIT_SOUND9,"swordHit0009", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_HIT_SOUND10,"swordHit00010", ".wav", 255);

	SoundLoad(Sound_ID::SWORD_WIND_SOUND1, "swordWind1", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_WIND_SOUND2, "swordWind2", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_WIND_SOUND3, "swordWind3", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_WIND_SOUND4, "swordWind4", ".wav", 255);
	SoundLoad(Sound_ID::SWORD_WIND_SOUND5, "swordWind5", ".wav", 255);

	SoundLoad(Sound_ID::JUST_AVOID_SOUND, "justAvoid10", ".wav", 200);
	SoundLoad(Sound_ID::ENEMY_ATTACK_BEFORE, "EnemyAttackBefore1", ".wav", 255);
	SoundLoad(Sound_ID::PLAYER_ATTACK_BEFORE, "PlayerAttackBefore", ".wav", 255);

	SoundLoad(Sound_ID::ENEMY_SWORD_WIND1, "EnemySword1", ".wav", 200);
	SoundLoad(Sound_ID::ENEMY_SWORD_WIND2, "EnemySword2", ".wav", 200);

	SoundLoad(Sound_ID::ENEMY_ATTACK_HIT1, "EnemyAttackHit1", ".wav", 200);
	SoundLoad(Sound_ID::ENEMY_ATTACK_HIT2, "EnemyAttackHit2", ".wav", 200);
	SoundLoad(Sound_ID::ENEMY_ATTACK_HIT3, "EnemyAttackHit3", ".wav", 200);
	SoundLoad(Sound_ID::ENEMY_ATTACK_HIT4, "EnemyAttackHit4", ".wav", 200);

	SoundLoad(Sound_ID::ENEMY_FALL, "EnemyFall2", ".wav", 255);

	SoundLoad(Sound_ID::SOUND_ID::V_P_ATTACK1, "P_AttackV1", ".wav", 180);
	SoundLoad(Sound_ID::SOUND_ID::V_P_ATTACK2, "P_AttackV2", ".wav", 180);
	SoundLoad(Sound_ID::SOUND_ID::V_P_ATTACK3, "P_AttackV3", ".wav", 180);
	SoundLoad(Sound_ID::SOUND_ID::V_P_ATTACK4, "P_AttackV4", ".wav", 255);
	SoundLoad(Sound_ID::SOUND_ID::V_P_LOSE, "P_V_AttackLose", ".wav", 255);

	SoundLoad(Sound_ID::V_P_DAMAGE1, "P_DamageV1", ".wav", 255);
	SoundLoad(Sound_ID::V_P_DAMAGE2, "P_DamageV2", ".wav", 255);

	SoundLoad(Sound_ID::V_P_JUST_AVOID, "P_JustAvoidV", ".wav", 255);
	SoundLoad(Sound_ID::V_P_AVOID, "P_AvoidV", ".wav", 255);

	SoundLoad(Sound_ID::V_E_ATTACK1, "E_AttackV1", ".wav",255);
	SoundLoad(Sound_ID::V_E_ATTACK2, "E_AttackV2", ".wav",255);
	SoundLoad(Sound_ID::V_E_ATTACK3, "E_AttackV3", ".wav",255);

	SoundLoad(Sound_ID::V_E_DAMAGE1, "E_DamageV1", ".wav", 200);
	SoundLoad(Sound_ID::V_E_DAMAGE2, "E_DamageV2", ".wav", 200);
	SoundLoad(Sound_ID::V_E_DAMAGE3, "E_DamageV3", ".wav", 200);
	SoundLoad(Sound_ID::V_E_DAMAGE4, "E_DamageV4", ".wav", 200);

	SoundLoad(Sound_ID::JUST_AVOID_SUCCESS, "success", ".wav", 255);

	SoundLoad(Sound_ID::PLAY_BGM, "PlaySceneBGM", ".wav", 150);
	
}

void SoundManager::TitleSceneLoad()
{

}

void SoundManager::CommonSceneLoad()
{
}

void SoundManager::PlaySe(Sound_ID::SOUND_ID _type)
{
	sound[Sound_ID::GetSoundID(_type)]->PlaySe();
}

void SoundManager::PlayBGM(Sound_ID::SOUND_ID _type, bool _loop, bool firstPlay)
{
	sound[Sound_ID::GetSoundID(_type)]->PlayBGM(_loop,firstPlay);
}

void SoundManager::StopBGM(Sound_ID::SOUND_ID _type)
{
	sound[Sound_ID::GetSoundID(_type)]->StopBGM();
}

void SoundManager::StopSE(Sound_ID::SOUND_ID _type)
{
	sound[Sound_ID::GetSoundID(_type)]->StopSE();
}

void SoundManager::AllDeleteSound()
{
	for (auto& s : sound) {
		delete s.second;
	}
	sound.clear();
}

void SoundManager::RandamSe(std::string _name, int num)
{
	int randomNum = GetRand(num - 1) + 1;

	std::string value = std::to_string(randomNum);

	std::string cats = _name + value;

	auto itr = sound.find(cats);
	if (itr == sound.end()) {
		Debug::DebugLog("このファイルは存在しません。");
	}
	sound[cats]->PlaySe();

}

bool SoundManager::CheckSe(Sound_ID::SOUND_ID _id)
{
	return CheckSoundMem(Load::GetSoundHandle(_id));
}

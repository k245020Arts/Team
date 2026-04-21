#include "Boss.h"
#include "../../Component/Animator/Animator.h"
#include "../../Component/Physics/Physics.h"
#include "../../Common/InputManager/PadInput.h"
#include "../../Common/InputManager/InputManager.h"
#include "../../Common/InputManager/KeyboardInput.h"
#include "../../Component/Collider/SphereCollider.h"
#include "../../Component/Color/Color.h"
#include "../../Component/Shaker/Shaker.h"
#include "../../../ImGui/imgui.h"
#include "../../Player/Player.h"
#include "../TrashEnemy/EnemyState/EnemyBlowAway.h"
#include "../../Camera/CameraManager.h"
#include "../../Camera/Camera.h"
#include "../../Common/Effect/EffectManager.h"
#include "../../Common/Sound/SoundManager.h"
#include "../../Weapon/WeaponManager.h"
#include "../../State/StateManager.h"
#include "../../GameControler/GameControler.h"
#include "../Boss/BossState/BossIdol.h"
#include "../Boss/BossState/BossRun.h"
#include "../Boss/BossState/BossWalk.h"
#include "../TrashEnemy/EnemyState/EnemyDamage.h"
#include "../../Player/PlayerState/AttackState/PlayerAttackStateBase.h"
#include "../../Player/PlayerState/AttackState/PlayerSpecialAttack.h"
#include "../../Weapon/SwordEffect.h"
#include "../../Weapon/CharaWeapon.h"
#include "../../Common/ResourceLoader.h"
#include "../Boss/BossState/AttackSorting.h"
#include "../Boss/BossState/BossCoolTime.h"
#include "../Boss/BossState/BossStatus.h"
#include "../Boss/BossState/BossDie.h"
#include "../Boss/BossState/BossLose.h"
#include "../Boss/BossState/BossRoar.h"
#include "../Boss/BossState/BossThreat.h"
#include "../Boss/BossState/BossDamage.h"
#include "../Boss/BossState/BossAppear.h"
#include "../Boss/BossState/BossFear.h"
#include "../Boss/BossState/BossBackStep.h"
#include "../Boss/BossState/BossWin.h"
#include "../TrashEnemy/TrashEnemyManager.h"//
#include "../../Common/Random.h"
#include "../../Component/UI/EnemyDamageUI.h"
#include "../../Component/EnemyAttackObject/BossRock/BossRockManager.h"
#include "../../Player/PlayerState/AttackState/PlayerAttack2.h"
#include "BossAttackDataSerializer.h"
#include "../../Stage/StageSelectData.h"
#include "../../Common/FileSystemUtils/FileSystemUtils.h"

namespace {
	std::unordered_map<StateID::State_ID, EnemyInformation::EnemyReaction> enemyTable;
}

//セーブ用
//JSON ToJson(const EnemyInformation::EnemyReaction& r) {
//	JSON j;
//	j["attackID"] = StateID::GetID(r.attackID);
//	//j["dInfo"] = r.dInfo;           // dInfo, bInfo も JSON に変換できるようにしておく
//	//j["bInfo"] = r.bInfo;
//	j["attackType"] = static_cast<int>(r.attackType);
//	j["changeStateID"] = StateID::GetID(r.changeStateID);
//	j["vibrationPower"] = r.vibrationPower;
//	j["vibrationType"] = r.vibrationType;
//	j["hitEffectID"] = Effect_ID::GetEffectID(r.hitEffectID);
//	j["hitEffectTime"] = r.hitEffectTime;
//	j["hitEffectScaleRate"] = r.hitEffectScaleRate;
//	j["hit"] = r.hit;
//	j["slashAngleRad"] = r.slashAngleRad;
//	j["slashEffectID"] = Effect_ID::GetEffectID(r.slashEffectID);
//	j["useSlashEffect"] = r.useSlashEffect;
//	j["loopMax"] = r.loopMax;
//	j["loopInterval"] = r.loopInterval;
//	return j;
//}

Boss::Boss()
{
	debugId = 19;
	tag = Function::GetClassNameC<Boss>();
	bs = new BossStatus;

	hp = bs->GetStatus().maxHp;
	defense = 500.0f;
	maxHp = hp;
	//trashEnemy = FindGameObject< TrashEnemyManager>();
	maxAttack = -1;
	comboFirstAttack = false;
	roaf = false;
	noAttackChangeCounter = 0.0f;
	alotAttack = 0;

	upPos = VECTOR3(0, 1500, 0);
	/*enemyBaseComponent.state->NowChangeState(StateID::BOSS_DIE_S);
	enemyBaseComponent.state->SetNoStateChange(true);*/
	oneDie = true;

	rockManager = new BossRockManager(this);
}

Boss::~Boss()
{
	//delete trashEnemy;
	//trashEnemy = nullptr;
	delete bs;
	bs = nullptr;
}

void Boss::Update()
{
	EnemyBase::Update();
	if (hp <= 0.0f && !enemyBaseComponent.camera->IsCutScene()) {
		if (oneDie) {
			enemyBaseComponent.state->SetNoStateChange(false);
			enemyBaseComponent.state->NowChangeState(StateID::BOSS_DIE_S);
			enemyBaseComponent.state->SetNoStateChange(true);
			oneDie = false;
		}
	
		
	}

	//if (obj->GetTransform()->position.y <= -10000.0f) {
	//	enemyBaseComponent.state->NowChangeState(StateID::BOSS_DIE_S);
	//	enemyBaseComponent.state->SetNoStateChange(true);
	//}

	if (InputManager::GetInstance()->GetKeyboardInput()->GetIsKeyboardPushing(KEY_INPUT_NUMPAD0)) {
		hp -= 200.0f;
	}

	/*if (CheckHitKey(KEY_INPUT_1)) {
		enemyBaseComponent.state->ChangeState(StateID::BOSS_SPECIAL_SMALL_ATTACK1_S);
	}

	if (CheckHitKey(KEY_INPUT_3)) {
		enemyBaseComponent.state->ChangeState(StateID::BOSS_SPECIAL_ATTACK2_S);
	}*/

	if (Hp() == FIVE) {
		if (!roaf) {
			enemyBaseComponent.state->ChangeState(StateID::B_ROAR_S);
			roaf = true;
			//obj->SetBaseTimeSpeed(1.2f);
			obj->SetObjectTimeRate();
		}
		
	}
	if (maxAttack >= 0) {
		player->BossThreatFinish();
	}
	if (player->GetBossThreat()) {
		if (threat) {
			enemyBaseComponent.state->ChangeState(StateID::B_THREAT_S);
			player->BossThreatFinish();
			threat = false;
		}
	}
	if (noAttackChangeCounter > 0.0f) {
		noAttackChangeCounter -= Time::DeltaTimeRate();
	}
	bossAttackDataSerializer->Update();
}

void Boss::Draw()
{
	EnemyBase::Draw();
}

EnemyInformation::EnemyReaction Boss::JsonRead(const JSON& j)
{
	EnemyInformation::EnemyReaction r;

	StateID::State_ID attack = StateID::StringToID(j["attackID"].get<std::string>());
	if (j.contains("attackID"))
		r.attackID = attack;
	if (j.contains("attackType"))
		r.attackType = static_cast<EnemyInformation::EnemyReaction::Type>(j["attackType"].get<int>());
	StateID::State_ID change = StateID::StringToID(j["changeStateID"].get<std::string>());
	if (j.contains("changeStateID"))
		r.changeStateID = change;
	if (j.contains("vibrationPower")) 
		r.vibrationPower = j["vibrationPower"].get<int>();
	if (j.contains("vibrationType"))
		r.vibrationType = j["vibrationType"].get<int>();
	Effect_ID::EFFECT_ID hit = Effect_ID::StringToID(j["hitEffectID"].get<std::string>());
	if (j.contains("hitEffectID"))
		r.hitEffectID = hit;
	if (j.contains("hitEffectTime"))
		r.hitEffectTime = j["hitEffectTime"].get<float>();
	if (j.contains("hitEffectScaleRate")) 
		r.hitEffectScaleRate = j["hitEffectScaleRate"].get<float>();
	if (j.contains("hit")) 
		r.hit = j["hit"].get<bool>();
	if (j.contains("slashAngleRad")) 
		r.slashAngleRad = j["slashAngleRad"].get<float>();
	Effect_ID::EFFECT_ID slash = Effect_ID::StringToID(j["slashEffectID"].get<std::string>());
	if (j.contains("slashEffectID")) 
		r.slashEffectID = slash;
	if (j.contains("useSlashEffect"))
		r.useSlashEffect = j["useSlashEffect"].get<bool>();
	if (j.contains("loopMax"))
		r.loopMax = j["loopMax"].get<int>();
	if (j.contains("loopInterval"))
		r.loopInterval = j["loopInterval"].get<float>();

	return r;
}

void Boss::Start(Object3D* _obj,const BossParam& _param) 
{
	enemyBaseComponent.state = obj->Component()->AddComponent<StateManager>();
	enemyBaseComponent.playerObj = FindGameObjectWithTag<Object3D>("PLAYER");
	pState = enemyBaseComponent.playerObj->Component()->GetComponent<Player>()->GetPlayerStateManager();
	player = enemyBaseComponent.playerObj->Component()->GetComponent<Player>();
	bossTransform = obj->GetTransform();

	enemyBaseComponent.collider = FindGameObjects<ColliderBase>();

	const StageData stageData = StageSelectData::GetInstance()->GetNowStageData();

	std::string charaID = "2";

	int num = stageData.bossID;

	//enemyBaseComponent.boss = this;
	enemyBaseComponent.anim = obj->Component()->GetComponent<Animator>();
	enemyBaseComponent.physics = obj->Component()->GetComponent<Physics>();
	enemyBaseComponent.color = obj->Component()->GetComponent<Color>();
	enemyBaseComponent.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
	enemyBaseComponent.shaker = obj->Component()->GetComponent<Shaker>();
	enemyBaseComponent.physics = obj->Component()->GetComponent<Physics>();

	enemyBaseComponent.camera = FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<Camera>();
	//enemyBaseComponent.weapon = FindGameObject<WeaponManager>();

	attackColl = CharaBase::CollsionSet(nullptr, CollsionInformation::SPHERE, "b_attack", true, CollsionInformation::B_ATTACK);
	justAvoidColl = CharaBase::CollsionSet(nullptr, CollsionInformation::SPHERE, "b_just_avoid", false, CollsionInformation::JUST_AVOID_BOSS);

	enemyBaseComponent.weapon = FindGameObject<WeaponManager>();

	enemyBaseComponent.gameManager = FindGameObject<GameControler>();

	using namespace ID;

	enemyBaseComponent.state->CreateState<BossIdol>("BossIdol", StateID::BOSS_IDOL_S);
	enemyBaseComponent.state->CreateState<BossRun>("BossRun", StateID::BOSS_RUN_S);
	enemyBaseComponent.state->CreateState<BossCoolTime>("BossCoolTime", StateID::BOSS_COOL_TIME_S);
	std::shared_ptr<AttackSorting> attackSorting =  enemyBaseComponent.state->CreateState<AttackSorting>("AttackSorting", StateID::ATTACK_SORTING_S);
	std::string folderName = "Boss" + std::to_string(num);
	attackSorting->Load(folderName,this);

	enemyBaseComponent.state->CreateState<BossWalk>("BossWalk", StateID::BOSS_WALK);
	/*enemyBaseComponent.state->CreateState<BossNormalAttack1>("BossNormalAttack1", StateID::BOSS_NORMAL_ATTACK1_S);
	enemyBaseComponent.state->CreateState<BossNormalAttack2>("BossNormalAttack2", StateID::BOSS_NORMAL_ATTACK2_S);
	enemyBaseComponent.state->CreateState<BossNormalAttack3>("BossNormalAttack3", StateID::BOSS_NORMAL_ATTACK3_S);
	enemyBaseComponent.state->CreateState<BossNormalAttack4>("BossNormalAttack4", StateID::BOSS_NORMAL_ATTACK4_S);
	enemyBaseComponent.state->CreateState<BossNormalAttack5>("BossNormalAttack5", StateID::BOSS_NORMAL_ATTACK5_S);
	enemyBaseComponent.state->CreateState<BossNormalAttack6>("BossNormalAttack6", StateID::BOSS_NORMAL_ATTACK6_S);
	enemyBaseComponent.state->CreateState<BossNormalAttack7>("BossNormalAttack7", StateID::BOSS_NORMAL_ATTACK7_S);
	enemyBaseComponent.state->CreateState<BossSpecialAttack1>("BossSpecialAttack1", StateID::BOSS_SPECIAL_ATTACK1_S);
	enemyBaseComponent.state->CreateState<BossSpecialSmallAttack1>("BossSpecialSmallAttack1", StateID::BOSS_SPECIAL_SMALL_ATTACK1_S);
	enemyBaseComponent.state->CreateState<BossSpecialAttack2>("BossSpecialAttack2", StateID::BOSS_SPECIAL_ATTACK2_S);*/
	enemyBaseComponent.state->CreateState<BossDie>("BossDie", StateID::BOSS_DIE_S);
	enemyBaseComponent.state->CreateState<BossRoar>("BossRoar", StateID::B_ROAR_S);
	enemyBaseComponent.state->CreateState<BossThreat>("BossThreat", StateID::B_THREAT_S);
	enemyBaseComponent.state->CreateState<BossDamage>("BossDamage", StateID::BOSS_DAMAGE_S);
	enemyBaseComponent.state->CreateState<BossAppear>("BossAppear", StateID::BOSS_APPEAR_S);
	enemyBaseComponent.state->CreateState<BossLose>("BossLose", StateID::BOSS_LOSE_S);
	//enemyBaseComponent.state->CreateState<BossHalfSpecialAttack>("BossHalfAttack", StateID::BOSS_HALF_ATTACK_S);
	enemyBaseComponent.state->CreateState<BossFear>("BossFear", StateID::BOSS_FEAR_S);
	enemyBaseComponent.state->CreateState<BossBackStep>("BossBackStep", StateID::BOSS_BACKSTEP_S);
	enemyBaseComponent.state->CreateState<BossWin>("BossWin", StateID::BOSS_WIN_S);
	enemyBaseComponent.state->SetComponent<Boss>(this);

	
	std::ostringstream oss;
	// 3桁、空きは'0'で埋める
	oss << std::setfill('0') << std::setw(3) << num;
	std::string typeID = oss.str();

	enemyBaseComponent.anim->AnimDataLoad(charaID, typeID);
	//enemyBaseComponent.anim->AnimDataLoad("BossAnimData");

	// 初期ステート
	enemyBaseComponent.state->StartState(StateID::BOSS_APPEAR_S);
	enemyBaseComponent.weapon = FindGameObject<WeaponManager>();
	chara = obj->Component()->AddComponent<CharaWeapon>();
	chara->ObjectPointer(_obj, 10, ID::B_MODEL, -1);
	chara->SetImage(ResourceLoad::GetHandle(ID::SWORD_EFFECT_B));

	JsonReader reader;
	if (reader.Load("data/json/Boss.json")) {
		const auto& arr = reader.Data();
		for (const auto& j : arr) {
			EnemyInformation::EnemyReaction r = JsonRead(j);
			// 配列に追加したり使用
			enemyTable[r.attackID] = r;
		}
	}

	bossAttackDataSerializer =  std::make_unique<BossAttackDataSerializer>(attackSorting,this,"Boss1");
	bossAttackDataSerializer->SetAnim(enemyBaseComponent.anim);
	bossAttackDataSerializer->SetThrowManager(rockManager);
	bossParam = _param;
	hp = bossParam.hp;
	defense = bossParam.defense;
	enemyBaseComponent.color->setRGB(Color::Rgb(255, 255, 255, 255));
	/*JsonReader json;
	std::string filePath = std::string("data/json/BossAttack");

	JSON root = nlohmann::json::object();

	root["Boss"] = nlohmann::json::array();
	root["Boss"].push_back(bossParam);
	
	json.Save(filePath + "/BossParam.json", root);*/
}

void Boss::ImguiDraw()
{
	if (ImGui::Button("bossDie")) {
		enemyBaseComponent.state->NowChangeState(StateID::BOSS_DIE_S);
		enemyBaseComponent.state->SetNoStateChange(true);
	}
}

void Boss::LookPlayer()
{
	LookPlayer(0.07f);
}

void Boss::LookPlayer(float speed)
{
	//プレイヤーのポジションを格納させる
	VECTOR3 targetPos = enemyBaseComponent.playerObj->GetTransform()->position;
	VECTOR3 distance = targetPos - bossTransform->position;
	//向くべき角度
	float direction = -atan2f(distance.z, distance.x) - 0.5f * DX_PI_F;
	//その角度とどれだけ差があるか
	float sign = direction - bossTransform->rotation.y;
	//切り捨てして180の境界線を無くす
	sign -= floorf(sign / DX_PI_F / 2) * DX_PI_F * 2.0f;
	if (sign > DX_PI_F)
		sign -= 2 * DX_PI_F;
	//向くスピード(ラジアン)
	const float LOOK_SPEED = speed;
	//Playerの方をゆっくり向く
	if (sign > LOOK_SPEED)
		bossTransform->rotation.y += LOOK_SPEED;
	else if (sign < -LOOK_SPEED)
		bossTransform->rotation.y -= LOOK_SPEED;
	else
		bossTransform->rotation.y = direction;
}

void Boss::LookPlayer(VECTOR3 _target, float speed)
{
	//プレイヤーのポジションを格納させる
	VECTOR3 targetPos = _target;
	VECTOR3 distance = targetPos - bossTransform->position;
	//向くべき角度
	float direction = -atan2f(distance.z, distance.x) - 0.5f * DX_PI_F;
	//その角度とどれだけ差があるか
	float sign = direction - bossTransform->rotation.y;
	//切り捨てして180の境界線を無くす
	sign -= floorf(sign / DX_PI_F / 2) * DX_PI_F * 2.0f;
	if (sign > DX_PI_F)
		sign -= 2 * DX_PI_F;
	//向くスピード(ラジアン)
	const float LOOK_SPEED = speed;
	//Playerの方をゆっくり向く
	if (sign > LOOK_SPEED)
		bossTransform->rotation.y += LOOK_SPEED;
	else if (sign < -LOOK_SPEED)
		bossTransform->rotation.y -= LOOK_SPEED;
	else
		bossTransform->rotation.y = direction;
}

void Boss::PlayerHit()
{
	//ID::IDType attackID = pState->GetState<PlayerStateBase>()->GetID();
	//float damage = 0;;
	//if (pState->GetState<PlayerAttackStateBase>() != nullptr) {
	//	damage = pState->GetState<PlayerAttackStateBase>()->GetHitDamage();
	//}
	//else {
	//	loopNum = -1;
	//}

	//EnemyDamage::EnemyDamageInfo dInfo;
	//EnemyBlowAway::EnemyBlowAwayInfo bInfo;
	//float random[3] = {};
	////エフェクトの位置をランダムに派生
	//for (float& r : random) {
	//	r = (float)GetRand(80) - 40.0f;
	//}
	////プレイヤーの攻撃の種類によってダメージや吹っ飛び方の種類を変える
	//float angleRan = 0.0f;
	//bool lastAttack = false;
	//bool lastBeforeAttack = false;
	//switch (attackID)
	//{
	//case ID::P_ANIM_ATTACK1:
	//	//enemyBaseComponent.state->NowChangeState( StateID::B_THREAT_S);
	//	dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 1500.0f), VECTOR3(100, 100, 100), 0.15f, 0.8f);
	//	enemyBaseComponent.control->ControlVibrationStartFrame(10, 10);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, -40.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
	//	hit = true;
	//	break;
	//case ID::P_ANIM_ATTACK2:
	//	//enemyBaseComponent.state->NowChangeState( StateID::B_THREAT_S);
	//	dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 1500.0f), VECTOR3(100, 100, 100), 0.25f, 0.8f);
	//	enemyBaseComponent.control->ControlVibrationStartFrame(40, 30);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, 40.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
	//	//enemyBaseComponent.effect->CreateEffekseer(Transform(VZero, VOne * VECTOR3(0, 0, 70.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
	//	hit = true;
	//	break;
	//case ID::P_ANIM_ATTACK3:
	//	//enemyBaseComponent.state->NowChangeState( StateID::B_THREAT_S);
	//	dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 1500.0f), VECTOR3(100, 100, 100), 0.35f, 0.8f);
	//	enemyBaseComponent.control->ControlVibrationStartFrame(10, 10);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, 80.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
	//	hit = true;
	//	break;
	//case ID::P_ANIM_ATTACK4:
	//	//enemyBaseComponent.state->NowChangeState( StateID::B_THREAT_S);
	//	dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 400.0f, 00.0f), VECTOR3(200, 200, 200), 0.85f, 1.0f);
	//	enemyBaseComponent.control->ControlVibrationStartFrame(50, 50);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, -120.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
	//	hit = true;
	//	break;
	//case ID::P_ANIM_JUST_AVOID_ATTACK1:
	//	enemyBaseComponent.state->NowChangeState( StateID::B_THREAT_S);
	//	obj->SetObjectTimeRate(1.0f);
	//	dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, -20000.0f, 2000.0f), VECTOR3(400, 400, 400), 0.05f, 0.8f);
	//	enemyBaseComponent.control->ControlVibrationStartFrame(30, 40);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
	//	//enemyBaseComponent.effect->CreateEffekseer(Transform(VZero, VOne * VECTOR3(0, 0, 50.0f * DegToRad), VOne), obj, ID::PLAYER_SLASH_ATTACK, 1.0f);
	//	break;
	//case ID::P_ANIM_JUST_AVOID_ATTACK1:
	//	
	//	break;
	//case ID::P_ANIM_ATTACK_5:
	//	//enemyBaseComponent.state->NowChangeState( StateID::B_THREAT_S);
	//	dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, -2000.0f, -2000.0f), VECTOR3(200, 200, 200), 0.2f, 0.8f);
	//	enemyBaseComponent.control->ControlVibrationStartFrame(70, 40);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, -50.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
	//	break;
	//case ID::P_ANIM_JUST_AVOID_ATTACK4:
	//	enemyBaseComponent.state->NowChangeState( StateID::B_THREAT_S);
	//	dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 7000.0f), VECTOR3(200, 200, 200), 0.2f, 0.8f);
	//	enemyBaseComponent.control->ControlVibrationStartFrame(40, 40);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, 80.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
	//	break;
	//case ID::P_ANIM_JUST_AVOID_ATTACK5:
	//	enemyBaseComponent.state->NowChangeState(ID::E_BLOWAWAY);
	//	bInfo = EnemyBlowAway::EnemyBlowAwayInfo(4000.0f, 3000.0f, VECTOR3(15.0f, 15.0f, 15.0f), VECTOR3(0.3f, 0.3f, 0.3f), 2000.0f, 20.0f);
	//	enemyBaseComponent.control->ControlVibrationStartFrame(100, 60);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::SPECIAL_HIT_EFFECT, 5.0f);
	//	enemyBaseComponent.effect->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, 0.0f * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
	//	break;
	//default:
	//	Debug::("PlayerNoAttackState");
	//}
	//EnemyDamageMove(dInfo);
	//hit = true;
	//hp -= damage;
	////ダメージか吹っ飛ばしの状態になっていたらダメージのパラメーターをいれる。
	//std::shared_ptr<EnemyDamage> eD = enemyBaseComponent.state->GetState<EnemyDamage>();
	//std::shared_ptr <EnemyBlowAway> eB = enemyBaseComponent.state->GetState<EnemyBlowAway>();
	//enemyBaseComponent.sound->RandamSe("E_DamageV", 2);
	//enemyBaseComponent.color->setRGB(Color::Rgb(255, 0, 0, 255));
	//damageFlash = 0.5f;
	/*if (eD != nullptr) {
		eD->DamageInit(dInfo);
	}
	else if (eB != nullptr) {
		eB->EnemyBlowAwayInfoSet(bInfo);
	}*/

	StateID::State_ID attackID = pState->GetState<PlayerStateBase>()->GetID();
	if (pState->GetState<PlayerAttack3>() != nullptr) {
		int c = 0;
	}

	float damage = 0;;
	if (pState->GetState<PlayerAttackStateBase>() != nullptr) {

		damage = pState->GetState<PlayerAttackStateBase>()->GetHitDamage();

		

	}
	else {
		loopNum = -1;
	}
	EnemyDamage::EnemyDamageInfo dInfo;
	EnemyBlowAway::EnemyBlowAwayInfo bInfo;
	float random[3] = {};
	//エフェクトの位置をランダムに派生
	for (float& r : random) {
		r = (float)GetRand(80) - 40.0f;
	}
	//プレイヤーの攻撃の種類によってダメージや吹っ飛び方の種類を変える
	float angleRan = 0.0f;
	bool lastAttack = false;
	bool lastBeforeAttack = false;
	auto bossParam = enemyTable.find(attackID);
	float angleRand = Random::GetFloat(0.0f, 360.0f);
	std::shared_ptr<PlayerSpecialAttack> pSpecial = pState->GetState<PlayerSpecialAttack>();
	Object2D* damageNum = nullptr;
	if (bossParam != enemyTable.end()) {
		const auto& e = bossParam->second;
		switch (e.attackType)
		{
		case EnemyInformation::EnemyReaction::Type::Normal:
			//Debug::DebugLog("hitToPlayerBoss");
			InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(e.vibrationPower, e.vibrationType);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne *e.hitEffectScaleRate), obj, e.hitEffectID, e.hitEffectTime);
			EffectManager::GetInstance()->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, e.slashAngleRad), VOne), obj, e.slashEffectID, 1.0f);
			hit = true;
			break;
		case EnemyInformation::EnemyReaction::Type::BlowAway:

			break;
		case EnemyInformation::EnemyReaction::Type::LoopCombo:
			SoundManager::GetInstance()->RandamSe("swordHit00000", 7);
			//攻撃の間隔の設定
			if (loopNum == -1) {
				hitCounter = 0.23f;
				loopNum = 5;
			}
			else if (loopNum == 1) {
				hitCounter = 0.13f;
				loopNum--;
				lastBeforeAttack = true;
			}
			else if (loopNum == 0) {
				loopNum = -1;
				lastAttack = true;
			}
			else if (loopNum > 0) {
				hitCounter = 0.13f;
				loopNum--;
			}
			//最後の攻撃
			if (lastAttack) {
				//enemyBaseComponent.state->NowChangeState( StateID::B_THREAT_S);
				dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 5000.0f), VECTOR3(100, 100, 100), 0.5f, 1.2f);
				InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(250, 60);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE * 14.5f), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
				//angleRan = GetRand(360);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, 90 * DegToRad), VOne * 1.5f), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
				hit = true;
			}
			else if (lastBeforeAttack) {//最後の一個前の攻撃
				dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(100, 100, 100), 0.5f, 0.5f);
				enemyBaseComponent.shaker->ShakeStart(VECTOR3(20, 20, 20), Shaker::HORIZONAL_SHAKE, true, 0.3f);
				InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(250, 60);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
				angleRan = (float)GetRand(360);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, 90.0f * DegToRad), VOne * 1.5f), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
			}
			else {//それ以外なら
				//enemyBaseComponent.state->NowChangeState( StateID::B_THREAT_S);
				dInfo = EnemyDamage::EnemyDamageInfo(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(20, 20, 20), 0.2f, 0.02f);
				InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(450, 20);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), VZero, VOne * EnemyInformation::HIT_EFFECT_SCALE_RATE), obj, Effect_ID::HIT_EFFECT, EnemyInformation::HIT_EFFECT_TIME);
				angleRan = (float)GetRand(360);
				EffectManager::GetInstance()->CreateEffekseer(Transform(VOne * VECTOR3(random[0] * 2.0f, 100, random[2]), VOne * VECTOR3(0, 0, angleRan * DegToRad), VOne), obj, Effect_ID::PLAYER_SLASH_ATTACK, 1.0f);
				enemyBaseComponent.playerObj->Component()->GetComponent<Shaker>()->ShakeStart(VECTOR3(200, 200, 200), Shaker::HORIZONAL_SHAKE, true, 0.05f);
				enemyBaseComponent.state->ChangeState(StateID::BOSS_DAMAGE_S);
			}
			if (maxAttack < 0) {
				enemyBaseComponent.state->ChangeState(StateID::BOSS_DAMAGE_S);
			}
			hit = true;
			break;
		case EnemyInformation::EnemyReaction::Type::Special:
			if (!specialAttackHit) {
				return;
			}
			
			if (pState->GetState<PlayerSpecialAttack>() != nullptr) {
				PlayerSpecialAttackHit(e,pSpecial, VECTOR3(random[0], 100 + random[1] / 5.0f, random[2]), angleRand);
			}
			specialAttackHit = false;
			break;
		default:
			break;
		}
	}
	EnemyDamageMove(dInfo);
	hp -= DamageCalculation(VECTOR3((float)(GetRand(400) - 200), (float)(800 + GetRand(400) - 200), (float)(GetRand(400) - 200)), damage ,500.0f, (float)GetRand(15));
	//ダメージか吹っ飛ばしの状態になっていたらダメージのパラメーターをいれる。
	std::shared_ptr<EnemyDamage> eD = enemyBaseComponent.state->GetState<EnemyDamage>();
	std::shared_ptr <EnemyBlowAway> eB = enemyBaseComponent.state->GetState<EnemyBlowAway>();
	SoundManager::GetInstance()->RandamSe("E_DamageV", 2);
	enemyBaseComponent.color->setRGB(Color::Rgb(255, 0, 0, 255));
	damageFlash = 0.25f;

}

void Boss::Drail(bool _right)
{
	if (_right) {
		chara->CreateSwordEffect(VECTOR3(70, 0, -50), VECTOR3(120, 0, 50), 200.0f, 10.0f, 00.0f, 155.0f, 28, 0.5f);
	}
	else {
		chara->CreateSwordEffect(VECTOR3(0, 0, -50), VECTOR3(50, 0, 100), 200.0f, 10.0f, 00.0f, 155.0f, 28, 0.5f);
	}
}

Boss::HP_RATE Boss::Hp()
{

	if (hp >= bs->GetStatus().maxHp * 0.8f) {
		hpRate = Boss::MAX;
	}
	else if (hp >= bs->GetStatus().maxHp * 0.5f) {
		hpRate = Boss::EIGHT;
	}
	else if (hp >= bs->GetStatus().maxHp * 0.3f) {
		hpRate = Boss::FIVE;
	}
	else {
		hpRate = Boss::THREE;
	}
	return hpRate;
}

void Boss::MoveBoss(float _speed, float _max)
{
	VECTOR3 dir = VZero;
	dir.y *= 0.0f;
	//b->bossTransform->GetRotationMatrix();
	dir = bossTransform->Forward() * -_speed;
	enemyBaseComponent.physics->AddVelocity(dir, false);
	VECTOR3 moveVelo;
	moveVelo = enemyBaseComponent.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);

	float max = _max;
	//最大速度までいったらスピードマックスに補正
	if (moveVelo.SquareSize() >= max * max) {
		moveVelo = moveVelo.Normalize() * max;
		moveVelo.y = enemyBaseComponent.physics->GetVelocity().y;
		enemyBaseComponent.physics->SetVelocity(moveVelo);
	}
}

float Boss::GetAttackCoolTime()
{
	switch (hpRate)
	{
	case Boss::MAX:
		coolTime = 0.3f;
		break;
	case Boss::EIGHT:
		coolTime = 0.2f;
		break;
	case Boss::FIVE:
		coolTime = 0.1f;
		break;
	case Boss::THREE:
		coolTime = 0.0f;
		break;
	default:
		break;
	}
	return coolTime;
}

void Boss::BossAttackStateChange()
{
	if (maxAttack != -1) {
		enemyBaseComponent.state->ChangeState(StateID::ATTACK_SORTING_S);
	}
	else {
		if (RunChangeAttack()) {
			enemyBaseComponent.state->ChangeState(StateID::ATTACK_SORTING_S);
			alotAttack++;
			if (alotAttack >= 3) {
				noAttackChangeCounter = Random::GetInt(1,2) * 0.5f;
				alotAttack = 0;
				enemyBaseComponent.state->ChangeState(StateID::BOSS_RUN_S);
			}
		}
		else {
			enemyBaseComponent.state->ChangeState(StateID::BOSS_RUN_S);
			alotAttack = 0;
		}
		
	}
}

bool Boss::RunChangeAttack()
{
	bool result = false;
	if (noAttackChangeCounter > 0.0f) {
		return result;
	}
	VECTOR3 targetVec = bossTransform->position - enemyBaseComponent.playerObj->GetTransform()->position;
	if (targetVec.Size() <= bs->GetStatus().range) {
		result = true;
	}
	return result;
}

void Boss::RockHitDamage(Physics* _phy)
{
	float damage = 500.0f;
	enemyBaseComponent.state->ChangeState(StateID::BOSS_DAMAGE_S);
	/*VECTOR3 baseSpeed = _phy->GetVelocity() * 1.0f;
	if (baseSpeed.Size() <= 3000.0f) {
		baseSpeed = baseSpeed.Normalize() * 3000.0f;
	}
	if (baseSpeed.Size() >= 6000.0f) {
		baseSpeed = baseSpeed.Normalize() * 6000.0f;
	}
	enemyBaseComponent.physics->AddVelocity(baseSpeed, false);*/
	hp -= DamageCalculation(VECTOR3((float)(GetRand(400) - 200), (float)(800 + GetRand(400) - 200), (float)(GetRand(400) - 200)), damage, 500.0f, (float)GetRand(15));
}

void Boss::RockHitRushDamage()
{
	float damage = 1000.0f;
	SoundManager::GetInstance()->PlaySe(Sound_ID::ROCK_BREAK);
	enemyBaseComponent.state->ChangeState(StateID::BOSS_FEAR_S);
	VECTOR3 baseSpeed = enemyBaseComponent.physics->GetVelocity() * -1.0f;
	EffectManager::GetInstance()->CreateEffekseer(Transform(VECTOR3(0,200,0), VZero, VOne), obj, Effect_ID::BOSS_ROCK_HIT_RUSH, 1.0f);
	if (baseSpeed.Size() <= 3000.0f) {
		baseSpeed = baseSpeed.Normalize() * 3000.0f;
	}
	if (baseSpeed.Size() >= 8000.0f) {
		baseSpeed = baseSpeed.Normalize() * 6000.0f;
	}
	enemyBaseComponent.physics->AddVelocity(baseSpeed, false);

	hp -= DamageCalculation(VECTOR3((float)(GetRand(400) - 200), (float)(800 + GetRand(400) - 200), (float)(GetRand(400) - 200)), damage, 500.0f, (float)GetRand(15));
}

void Boss::PlayerSpecialAttackHit(const EnemyInformation::EnemyReaction& _e, std::shared_ptr<PlayerSpecialAttack> _ps, VECTOR3 _randomPos, float _randomAngle)
{
	PlayerSpecialAttack::PLAYER_SPECIAL_ATTACK_STATE state = _ps->GetSpecialAttackState();

	switch (state)
	{
	case PlayerSpecialAttack::NO_ATTACK:
		break;
	case PlayerSpecialAttack::BEFORE:
		break;
	case PlayerSpecialAttack::GROUND_ATTACK:
		InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(_e.vibrationPower, _e.vibrationType);
		EffectManager::GetInstance()->CreateEffekseer(Transform(_randomPos, _randomAngle * DegToRad, VOne * _e.hitEffectScaleRate), obj, _e.hitEffectID, _e.hitEffectTime);
		EffectManager::GetInstance()->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, _e.slashAngleRad), VOne), obj, _e.slashEffectID, 1.0f);
		enemyBaseComponent.state->ChangeState(StateID::BOSS_DAMAGE_S);
		specialAttackHit = false;
		break;
	case PlayerSpecialAttack::CHARGE:
		EffectManager::GetInstance()->CreateEffekseer(Transform(_randomPos, _randomAngle * DegToRad, VOne * _e.hitEffectScaleRate), obj, _e.hitEffectID, _e.hitEffectTime);
		EffectManager::GetInstance()->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, _e.slashAngleRad), VOne), obj, _e.slashEffectID, 1.0f);
		enemyBaseComponent.state->ChangeState(StateID::BOSS_DAMAGE_S);
		enemyBaseComponent.shaker->ShakeStart(VECTOR3(100, 100, 100), Shaker::HORIZONAL_SHAKE, true, 2.0f);
		break;
	case PlayerSpecialAttack::FINAL_ATTACK:
		InputManager::GetInstance()->GetControllerInput()->ControlVibrationStartFrame(_e.vibrationPower, _e.vibrationType);
		EffectManager::GetInstance()->CreateEffekseer(Transform(_randomPos, _randomAngle * DegToRad, VOne * _e.hitEffectScaleRate), obj, _e.hitEffectID, _e.hitEffectTime);
		EffectManager::GetInstance()->CreateEffekseer(Transform(VOne * VECTOR3(0, 100, 0), VOne * VECTOR3(0, 0, _e.slashAngleRad), VOne), obj, _e.slashEffectID, 1.0f);
		enemyBaseComponent.state->ChangeState(StateID::BOSS_DAMAGE_S);
		enemyBaseComponent.physics->SetVelocity(VECTOR3(0.0f,0.0f,-3000.0f) * obj->GetTransform()->Forward());
		break;
	default:
		break;
	}
}
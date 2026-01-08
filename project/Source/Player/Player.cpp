#include "player.h"
#include "../State/StateManager.h"
#include "../Player/PlayerState/PlayerStateManager.h"
#include "../Component/MeshRenderer/MeshRenderer.h"
#include "../Component/Physics/Physics.h"
#include "../Camera/Camera.h"
#include "../Common/InputManager/InputManager.h"
#include "../Common/InputManager/controllerInputManager.h"
#include "../Common/InputManager/keyboardInputManager.h"
#include "../../ImGui/imgui.h"
#include "../Component/Transform/Transform.h"
#include "../Component/Animator/Animator.h"
#include "../Camera/cameraManager.h"
#include "../Player/PlayerState/PlayerStateBase.h"
#include "../Component/ComponentManager.h"
#include "../Player/PlayerState/PlayerAvoid.h"
#include "../Component/Shaker/Shaker.h"
#include "../Common/Sound/SoundManager.h"
#include "../Common/Effect/EffectManager.h"
#include "../Weapon/WeaponManager.h"
#include "../Component/MotionBlur/MotionBlur.h"
#include "../Player/PlayerState/PlayerWait.h"
#include "../Player/PlayerState/playerWalk.h"
#include "../Player/PlayerState/AttackState/playerAttack1.h"
#include "../Player/PlayerState/AttackState/playerAttack2.h"
#include "../Player/PlayerState/playerDamage.h"
#include "../Player/PlayerState/playerjustAvoid.h"
#include "../Player/PlayerState/PlayerBlowAway.h"
#include "../Player/PlayerState/PlayerTurn.h"
#include "../Player/PlayerState/AttackState/playerJustAvoidAttack1.h"
#include "../Player/PlayerState/AttackState/playerJustAvoidAttack2.h"
#include "../Player/PlayerState/AttackState/playerAttack3.h"
#include "../Player/PlayerState/AttackState/playerAttack4.h"
#include "../Player/PlayerState/AttackState/playerJustAvoidAttack3.h"
#include "../Player/PlayerState/AttackState/playerJustAvoidAttack4.h"
#include "../Player/PlayerState/AttackState/playerJustAvoidAttack5.h"
#include "../Player/PlayerState/AttackState/PlayerSpecialAttack.h"
#include "../Common/LoadManager.h"
#include "../Enemy/TrashEnemy/Enemy.h"
#include "../Enemy/TrashEnemy/EnemyState/EnemyStateManager.h"
#include "../Enemy/TrashEnemy/EnemyState/AttackState/EnemyAttack1.h"
#include "../Common/Transitor/FadeTransitor.h"
#include "../GameManager/GameManager.h"
#include "../Player/PlayerState/PlayerDie.h"
#include "../Enemy/EnemyManager.h"
#include "../Enemy/Boss/BossState/Attack/BossAttackBase.h"
#include "../Common/Easing.h"


namespace {

	std::unordered_map<StateID::State_ID, PlayerInformation::PlayerReaction> attackEffects = {
	{ StateID::PLAYER_ATTACK1_S, { VECTOR3(50,50,50), 0.07f, VECTOR3(40,40,40), 0.1f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE } },
	{ StateID::PLAYER_JUST_AVOID_ATTACK1_S, { VECTOR3(100,100,100), 0.15f, VECTOR3(100,100,100), 0.3f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE } },
	{  StateID::PLAYER_ATTACK2_S, { VECTOR3(50,50,50), 0.1f, VECTOR3(40,40,40), 0.1f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE } },
	{  StateID::PLAYER_ATTACK3_S, { VECTOR3(50,50,50), 0.12f, VECTOR3(40,40,40), 0.1f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE } },
	{ StateID::PLAYER_JUST_AVOID_ATTACK3_S, { VECTOR3(150,100,100), 0.05f, VECTOR3(30,50,50), 0.3f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE } },
	{  StateID::PLAYER_ATTACK4_S, { VECTOR3(100,100,100), 0.2f, VECTOR3(100,100,100), 0.3f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE } },
	{ StateID::PLAYER_JUST_AVOID_ATTACK4_S, { VECTOR3(100,100,100), 0.15f, VECTOR3(100,100,100), 0.3f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE } },
	{ StateID::PLAYER_JUST_AVOID_ATTACK5_S, { VECTOR3(100,100,100), 0.3f, VECTOR3(100,100,100), 0.3f, "swordHit00000", 7, true, Shaker::HORIZONAL_SHAKE } },
	};


}

Player::Player()
{
	playerCom.stateManager = nullptr;
	avoidReady = false;
	size = 0.00f;
	avoidStart = false;
	enemyHit = false;
	justAvoid = false;
	justAvoidCanCounter = 0.0f;
	avoidCounter = 0;
	noAvoidCounter = 0.0f;
	playerTransform = nullptr;
	debugId = 14;
	tag = Function::GetClassNameC<Player>();
	hp = MAX_HP;
	maxHp = hp;
	avoidReadyCounter = 0.0f;
	justAvoidBlurImage = Load::LoadImageGraph(Load::IMAGE_PATH + "visionEffect", ID::JUST_AVOID_BLUR);
	justFeedInTime = 0.0f;
	justFeedOutTime = 0.0f;
	bossThreat = false;
	largeJustAvoid = false;
	noDamage = false;
	redCounter = 0.0f;
	playerCom = PlayerInformation::CharaComponent();
	turn = false;
	specialAttackCenterPos = VZero;
	specialAttackStartPos = VZero;
	specialAttackBar = 0.0f;
	specialAttackBarMax = 100.0f;
}
//
//Object2D* guage = new Object2D();
//
//guage->Init(VECTOR2F(150, 115), VECTOR2F(0.0f, 0.0f), VECTOR2F(0.1f, 0.1f), "TrashEnemyHpGuage");
//
//e->AddChild(guage);
//
//Guage* g = guage->Component()->AddComponent<Guage>();
//g->EdgeDrawReady(Load::LoadImageGraph(Load::IMAGE_PATH + "bossHpEdge1", ID::BOSS_HP_EDGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F, Transform(VECTOR3(915.0f, 120.0f, 0.0f), VZero, VECTOR3(0.2f, 0.2f, 0.2f)));
//g->GuageDrawReady<TrashEnemy>(Load::LoadImageGraph(Load::IMAGE_PATH + "playerHp", ID::PLAYER_HP_GUAGE), MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F);
//g->WorldToScreenMode(true, VECTOR3(0, 500, 0));

Player::~Player()
{
	//delete playerCom.stateManager;
	/*DeleteGraph(justAvoidBlurImage);*/
	justAvoidBlurImage = -1;
}

void Player::Update()
{
	//playerCom.stateManager->Update();
	//回避状態が始まるときに回転に補正を掛けるための処理
	playerTransform->position = playerCom.anim->BoneMovePositionAdd(playerTransform->position);
	if (avoidReady) {
		AvoidRotationChange();
	}
	//ジャスト回避が出来るようになる
	if (enemyHit) {
		JustAvoidCan();
	}
	//攻撃のあたりはんていが終わったら削除
	if (playerCom.stateManager->GetState<PlayerAttackStateBase>() != nullptr) {
		if (!playerCom.stateManager->GetState<PlayerAttackStateBase>()->IsAttack()) {
			DeleteCollision();
		}
	}

	if (playerCom.keyboard->GetIsKeyboardPut(KEY_INPUT_2)){
		playerCom.stateManager->ChangeState(StateID::PLAYER_SPECIAL_ATTACK_S);
	}
	if (playerCom.keyboard->GetIsKeyboardPushing(KEY_INPUT_4)) {
		SpecialVarAdd(20.0f);
	}
	if (playerCom.keyboard->GetIsKeyboardPushing(KEY_INPUT_6)) {
		playerCom.camera->CutSceneChangeState("test");
	}
	
	//死亡条件
	if (hp <= 0.0f && (playerCom.stateManager->GetState<StateBase>()->GetID() == StateID::PLAYER_WAIT_S)) {
		playerCom.stateManager->NowChangeState(StateID::PLAYER_DIE_S);
		playerCom.stateManager->SetNoStateChange(true);
	}

	if (obj->GetTransform()->position.y <= -10000.0f) {
		playerCom.stateManager->NowChangeState(StateID::BOSS_DIE_S);
		playerCom.stateManager->SetNoStateChange(true);
	}
	if (redCounter > 0.0f) {
		redCounter -= Time::DeltaTimeRate();
		if (redCounter <= 0.0f) {
			redCounter = 0.0f;
			playerCom.color->setRGB(Color::Rgb(0.0f, 0.0f, 0.0f, 255.0f));
		}
	}
	//playerCom.physics->AddVelocity(VECTOR3(50.0f, 0.0f, 0.0f), false);
}

void Player::Draw()
{
	playerCom.stateManager->Draw();
	playerCom.renderer->Draw();

	//DrawLine3D(playerCom.camera->GetCameraTransform()->position, playerCom.camera->GetCameraTransform()->position + VECTOR3(0, 0, 1000) * obj->GetTransform()->GetRotationMatrix(), 0xffffff);

}

void Player::Start(Object3D* _obj)
{
	//初期化の値を設定
	//必要なコンポーネントを付けている。
	obj						= _obj;
	playerCom.stateManager	= obj->Component()->GetComponent<StateManager>();
	

	playerCom.player		= this;

	ComponentManager* c		= obj->Component();
	
	playerCom.renderer		= c->GetComponent<MeshRenderer>();
	playerCom.physics		= c->GetComponent<Physics>();

	playerCom.camera		= FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<Camera>();

	//playerCom.InputManager = FindGameObject<ImputManager>();
	playerCom.controller	= FindGameObject<ControllerInputManager>();
	playerCom.keyboard		= FindGameObject<KeyboardInputManager>();
	playerCom.InputManager	= FindGameObject<InputManager>();

	playerTransform			= obj->GetTransform();

	playerCom.anim			= obj->Component()->GetComponent<Animator>();
	playerCom.anim->Play(ID::P_ANIM_RUN);
	playerCom.color			= obj->Component()->GetComponent<Color>();
	//playerCom.targetObj = FindGameObjectWithTag<Object3D>("ENEMY");
	playerCom.targetObj		= nullptr;
	playerCom.hitObj		= nullptr;
	playerCom.shaker		= c->GetComponent<Shaker>();

	playerCom.effect		= FindGameObject<EffectManager>();
	playerCom.sound			= FindGameObject<SoundManager>();

	playerCom.weapon		= FindGameObject<WeaponManager>();
	playerCom.blur			= obj->Component()->GetComponent<MotionBlur>();

	playerCom.gameManager	= FindGameObject<GameManager>();
	playerCom.enemyManager	= FindGameObject<EnemyManager>();
	
	avoidStart				= false;
	justAvoidCanCounter		= 0.0f;
	attackColl				= nullptr;
	collName				= "p_attack";
	//playerCom.physics->SetVelocity(VECTOR3(10.0f, 5.0f, 0.0f));

	//physics->SetInterect(VECTOR3(5.0f, -1.0f, 0.0f),0.1);
	using namespace ID;
	//ステートのセット
	

	playerCom.stateManager->NodeDrawReady();
	playerCom.stateManager->SetComponent<Player>(this);

	playerCom.stateManager->StartState(StateID::PLAYER_WAIT_S);
	redCounter = 0.0f;
}

void Player::Move(float _speed, float _speedMax)
{
	//if (CheckHitKey(KEY_INPUT_RIGHT)) {
	//	//VECTOR3 power = com.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);
	//	//if (power.Size() <= 1.0f) {
	//	playerCom.physics->AddVelocity(VECTOR3(50.0f, 0.0f, 0.0f), false);
	//	//}

	//}
	PlayerStickInput();
	if (CheckHitKey(KEY_INPUT_A)) {
		//VECTOR3 power = com.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);
		//if (power.Size() >= -1.0f) {
		walkAngle.x = -1.0f;
		//}
	}
	else if (CheckHitKey(KEY_INPUT_W)) {
		walkAngle.z = 1.0f;
	}
	else if (CheckHitKey(KEY_INPUT_S)) {
		walkAngle.z = -1.0f;
	}
	else if (CheckHitKey(KEY_INPUT_D)) {
		walkAngle.x = 1.0f;
	}

	
	std::shared_ptr<PlayerStateBase> pB = playerCom.stateManager->GetState<PlayerStateBase>();

	StickDirections stick = playerCom.controller->GetStickKnockingReverce(0.6f, 8).leftStick;
	StickDirections nowStick = playerCom.controller->GetStickKnocking(0.6f, 1).leftStick;
	
	//スティックの傾きの量が少なかったら移動しない
	if ((fabs(walkAngle.x) >= 0.3f || fabs(walkAngle.z) >= 0.3f) && hp > 0.0f) {
		//回転処理
		RotationChange(walkAngle,12.0f);

		VECTOR3 dir = VZero;
		dir.x		= walkAngle.x * 1.0f * _speed;
		dir.z		= walkAngle.z * 1.0f * _speed;
		dir.y		*= 0.0f;
		dir			= dir * MGetRotY(playerCom.camera->GetCameraTransform()->rotation.y);
		playerCom.physics->AddVelocity(dir, false);
		VECTOR3 moveVelo;
		moveVelo	= playerCom.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);
		
		float max	= _speedMax;
		size		= moveVelo.SquareSize();

		//最大速度までいったらスピードマックスに補正
		if (moveVelo.SquareSize() >= max * max) {
			moveVelo = moveVelo.Normalize() * _speedMax;
			moveVelo.y = playerCom.physics->GetVelocity().y;
			playerCom.physics->SetVelocity(moveVelo);
		}
		playerCom.stateManager->ChangeState(StateID::PLAYER_WALK_S);
		//アニメーションのスピードを傾き方で測定
		playerCom.anim->SetPlaySpeed(walkAngle.Size());

		
		/*if (nowStick == S_NO_DIRECTION || stick == S_NO_DIRECTION) {
			return;
		}
		if (nowStick == stick) {
			playerCom.stateManager->ChangeState(StateID::PLAYER_TURN_S);
		}*/
	}
	else {
		playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
	}
	
}

void Player::RotationChange(VECTOR3 _angle,float _speed)
{
	VECTOR3 forward = VECTOR3(0, 0, 1) * MGetRotY(playerTransform->rotation.y);
	VECTOR3 right	= VECTOR3(1, 0, 0) * MGetRotY(playerTransform->rotation.y);
	VECTOR3 target	= _angle * MGetRotY(playerCom.camera->GetCameraTransform()->rotation.y);
	float dot = VDot(target.Normalize(), forward.Normalize());	//コサインの値が正面ベクトルとカメラの角度を計算
	//内積を使って補正
	if (dot >= cosf(_speed * DegToRad)) {
		float inRot = atan2f(target.x, target.z);
		playerTransform->rotation.y = inRot;
		avoidStart					= true;
	}
	else{
		playerTransform->rotation.y = (VDot(right,target) > 0) ? playerTransform->rotation.y + _speed * DegToRad :
			playerTransform->rotation.y - _speed * DegToRad;
		avoidStart = false;
	}
}

void Player::RotationChange()
{
	RotationChange(walkAngle,5.0f);
}

void Player::Avoid(float _speed, float _speedMax, float cameraAngle, float _upSpeed)
{
	//回避の共通処理
	VECTOR dir	= VZero;
	dir			= VECTOR3(0.0f, 0.0f, _speed) * MGetRotY(playerTransform->rotation.y);
	/*dir.x = playerTransform->rotation.y * 1.0f * _speed;
	dir.z = playerTransform->rotation.y * 1.0f * _speed;*/
	dir.y		*= 0.0f;
	//dir = dir * MGetRotY(cameraAngle);
	dir.y		+= _upSpeed;
	playerCom.physics->SetVelocity(dir);

}

void Player::ImguiDraw()
{
	ImGui::InputFloat("size", &size);
	ImGui::Text(playerCom.stateManager->GetState<StateBase>()->GetString().c_str());
	//playerCom.stateManager->StateNodeDraw();
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
	/*bool open = ImGui::TreeNodeEx("PlayerState", flags);
	if(open){
		if (ImGui::Button("p_Damage")) {
			playerCom.stateManager->ChangeState(ID::P_DAMAGE);
		}
		if (ImGui::Button("p_attack1")) {
			playerCom.stateManager->ChangeState(ID::P_ANIM_ATTACK1);
		}
		if (ImGui::Button("p_attack2")) {
			playerCom.stateManager->ChangeState(ID::P_ANIM_ATTACK2);
			}
		if (ImGui::Button("p_blowWay")) {
			playerCom.stateManager->ChangeState(ID::P_BLOWAWAY);
		}
		if (ImGui::Button("justAvoid")) {
			playerCom.stateManager->ChangeState(ID::P_ANIM_JUST_AVOID);
		}
		if (ImGui::Button("playerDie")) {
			playerCom.stateManager->NowChangeState(ID::P_DIE);
			playerCom.stateManager->SetNoStateChange(true);
		}
		ImGui::TreePop();
	}*/

	//static int screen = MakeScreen(256, 256, TRUE);
	//static int modelHandle = Load::GetHandle(ID::P_MODEL);

	//// モデル描画先をレンダーターゲットに
	//SetDrawScreen(screen);
	//ClearDrawScreen();
	//MV1DrawModel(modelHandle);

	//SetDrawScreen(DX_SCREEN_BACK); // 元に戻す

	//// D3D11 のテクスチャを取得
	//ID3D11ShaderResourceView* srv = GetUseDirect3D11ShaderResourceView(modelHandle);

	//// ImGui で画像を表示
	//ImGui::Image((ImTextureID)modelHandle, ImVec2(256, 256));

}

void Player::PlayerStickInput()
{
	//スティックの角度とり
	walkAngle = VECTOR3(playerCom.controller->GetStickInput().leftStick.x, 0.0f, playerCom.controller->GetStickInput().leftStick.y);
}

void Player::AvoidReady()
{
	/*if (avoidCounter <= 0) {
		return;
	}*/
	//回避のスタート処理
	PlayerStickInput();
	avoidReady			= true;
	justAvoidCanCounter = 0.15f;
	avoidCounter--;
	noAvoidCounter		= 2.0f;
	avoidReadyCounter	= 0.00f;
}

void Player::AvoidRotationChange()
{
	//回避するときにどれくらいの速度で回転させるかを示す処理
	RotationChange(walkAngle,20.0f);
	if (avoidReadyCounter >= 0.0f) {
		avoidReadyCounter	-= Time::DeltaTimeRate();
	}
	else {
		justAvoidCanCounter -= Time::DeltaTimeRate();
	}
	if (avoidStart && justAvoidCanCounter <= 0.0f && avoidReadyCounter <= 0.0f) {
		playerCom.stateManager->ChangeState(StateID::PLAYER_AVOID_S);
		avoidStart	= false;
		avoidReady	= false;
		justAvoid	= false;
		
	}
}

bool Player::EnemyHit(ID::IDType _attackId,BaseObject* _obj)
{
	if (noDamage) {
		return true;
	}
	//敵の攻撃が当たった時の処理
	std::shared_ptr<StateBase> pB	= playerCom.stateManager->GetState<StateBase>();
	Animator* enemyAnim				= _obj->Component()->GetComponent<Animator>();
	std::shared_ptr<BossAttackBase> attack = _obj->Component()->GetComponent<StateManager>()->GetState<BossAttackBase>();
	float startTime					= enemyAnim->EventStartTime(_attackId);
	bool damage						= false;
	if (attack == nullptr)
		return true;

	BossAttackBase::DamagePattern param = attack->GetDamageParam();
	//ジャスト回避が出来る処理
	if (justAvoidCanCounter > 0.0f && avoidReadyCounter <= 0.0f) {
		if (enemyAnim->GetCurrentFrame() <= startTime + 3.0f || startTime >= 0.0f) {
			/*if (!LargeJustAvoid(attack)) {
				return true;
			}*/
			playerCom.stateManager->ChangeState(StateID::PLAYER_JUST_AVOID_S);
			playerCom.hitObj	= _obj;
			playerCom.enemyManager->JustAvoidTargetChange(dynamic_cast<Object3D*>(_obj));
			playerCom.camera->TargetSet(_obj);
			Debug::DebugLog("JustAvoid");
			avoidStart			= false;
			avoidReady			= false;
			justAvoidCanCounter = 0.0f;
			justAvoid			= true;
			justFeedInTime		= JUST_FEED_IN_TIME;
			SpecialVarAdd(10.0f);
		}
	}
	else {
		//ジャスト回避が出来るようになったらスルー
		if (enemyAnim->GetCurrentFrame() <= startTime + 2.0f && attack != nullptr) {
			return false;
		}
		else {
			//出来なかったらダメージを食らう
			damage = true;
		}
		
	}
	//ダメージが入ったらパラメーターのセット
	if (damage) {
		if (pB->GetID() != ID::P_ANIM_AVOID) {
			playerCom.controller->ControlVibrationStartFrame(80, 30);
			//playerCom.stateManager->ChangeState(StateID::PLAYER_DAMAGE_S);
			if (attack == nullptr) {
				hp -= 50.0f;
			}
			else {
				hp -= param.hitDamage;
			}
			switch (param.damagePattern)
			{
			case BossAttackBase::NO_BACK:
				playerCom.stateManager->ChangeState(StateID::PLAYER_DAMAGE_S);
				break;
			case BossAttackBase::BACK:
				playerCom.stateManager->ChangeState(StateID::PLAYER_DAMAGE_S);
				break;
			case BossAttackBase::BLOW_AWAY:
				playerCom.stateManager->ChangeState(StateID::PLAYER_BLOW_AWAY_S);
				break;
			default:
				my_error_assert("ダメージの状態が入っていません");
				break;
			}
			playerCom.color->setRGB(Color::Rgb(255.0f, 0.0f, 0.0f, 255.0f));
			redCounter = 0.5f;
			playerCom.hitObj = _obj;
			//hp -= playerCom.hitObj->Component()->GetComponent<Enemy>()->GetStateManager()->GetState<EnemyAttack1>()->GetHitDamage();
			playerCom.sound->RandamSe("EnemyAttackHit",4);
			playerCom.sound->RandamSe("P_DamageV",2);
			playerCom.effect->CreateEffekseer(Transform(VECTOR3(0,50,0),VZero,VOne * 6.0f), obj, Effect_ID::PLAYER_HIT, 1.0f);
		}
	}
	return true;
}

void Player::JustAvoidCan()
{
}

void Player::TargetObjSet(BaseObject* _base)
{
	if (_base == nullptr) {
		playerCom.camera->TargetSet(nullptr);
	}
	playerCom.targetObj = _base;
}

void Player::HitObjectSet(BaseObject* _base)
{
	playerCom.hitObj = _base;
}

void Player::PlayerAttackHit()
{
	//プレイヤーの攻撃を当てた時のカメラのシェイクヒットストップの処理
	StateID::State_ID id = playerCom.stateManager->GetState<StateBase>()->GetID();

	auto it = attackEffects.find(id);
	if (it != attackEffects.end()) {
		const auto& e = it->second;
		playerCom.shaker->ShakeStart(e.shakePower, e.shakePattern, e.shakerLoop, e.shakeTime);
		playerCom.camera->CameraShake(e.cameraShakePower, e.shakePattern, false, e.cameraShakeTime);
		playerCom.sound->RandamSe(e.soundName, e.soundKind);
		SpecialVarAdd(1.0f);
	}
}

bool Player::IsShake() 
{
	return playerCom.shaker->IsShakeing();
}

StateManager* Player::GetPlayerStateManager()
{
	return playerCom.stateManager;
}

void Player::AvoidFinishState()
{
	PlayerStickInput();
	//スティックをある程度傾けていたら移動の処理、傾けていなかったら止まる処理
	if (fabs(walkAngle.x) >= 0.3f || fabs(walkAngle.z) >= 0.3f) {
		playerCom.stateManager->ChangeState(StateID::PLAYER_WALK_S);
	}
	else {
		playerCom.stateManager->ChangeState(StateID::PLAYER_WAIT_S);
	}
}

void Player::DrawTrail()
{
	DrawTrail(VECTOR3(-23, -4, -200), VECTOR3(23, 4, 16), 0.0f, 0.0f, 255.0f, 100.0f, 28, 0.25f);
}

void Player::DrawTrail(VECTOR3 _nPos, VECTOR3 _fPos, float _r, float _g, float _b, float _a, float index, float _time)
{
	//剣の軌跡の処理
	playerCom.weapon->CreateTrailPlayer(_nPos,_fPos,_r,_g,_b,_a,index,_time);
}

void Player::DeleteCollision()
{
	CharaBase::DeleteCollision();
}

bool Player::EnemyAttackObjectHitIsPlayer(BaseObject* _obj)
{
	//敵の攻撃が当たった時の処理
	if (noDamage) {
		return true;
	}
	std::shared_ptr<StateBase> pB = playerCom.stateManager->GetState<StateBase>();
	bool damage = false;
	playerCom.hitObj = _obj;
	//ジャスト回避が出来る処理
	if (justAvoidCanCounter > 0.0f && avoidReadyCounter <= 0.0f) {
		
		playerCom.stateManager->ChangeState(StateID::PLAYER_JUST_AVOID_S);
		Debug::DebugLog("JustAvoid");
		avoidStart			= false;
		avoidReady			= false;
		justAvoidCanCounter = 0.0f;
		justAvoid			= true;
		justFeedInTime		= JUST_FEED_IN_TIME;
		largeJustAvoid		= true;
	}
	else {	
		//出来なかったらダメージを食らう
		damage = true;
	}
	//ダメージが入ったらパラメーターのセット
	if (damage) {
		if (pB->GetID() != StateID::PLAYER_AVOID_S) {
			playerCom.controller->ControlVibrationStartFrame(80, 30);
			playerCom.stateManager->ChangeState(StateID::PLAYER_DAMAGE_S);
			hp -= 50.0f;
			//hp -= playerCom.hitObj->Component()->GetComponent<Enemy>()->GetStateManager()->GetState<EnemyAttack1>()->GetHitDamage();
			playerCom.sound->RandamSe("EnemyAttackHit", 4);
			playerCom.sound->RandamSe("P_DamageV", 2);
			playerCom.effect->CreateEffekseer(Transform(VECTOR3(0, 50, 0), VZero, VOne * 6.0f), obj, Effect_ID::PLAYER_HIT, 1.0f);
		}
	}
	return true;
}

bool Player::LargeJustAvoid(std::shared_ptr<BossAttackBase> _attack)
{
	if (_attack->GetDamageParam().flash) {
		largeJustAvoid = true;
	}
	else {
		largeJustAvoid = false;
	}
	return largeJustAvoid;
}

void Player::SpecialVarAdd(float _add)
{
	float add = specialAttackBar + _add;
	specialAttackBar = std::clamp(add, 0.0f, specialAttackBarMax);
}

#include "player.h"
#include "playerStateManager.h"
#include "MeshRenderer.h"
#include "Physics.h"
#include "camera.h"
#include "inputManager.h"
#include "controllerInputManager.h"
#include "keyboardInputManager.h"
#include "../ImGui/imgui.h"
#include "transform.h"
#include "Animator.h"
#include "cameraManager.h"
#include "playerStateBase.h"
#include "ComponentManager.h"
#include "playerAvoid.h"
#include "Shaker.h"
#include "SoundManager.h"
#include "EffectManager.h"
#include "weaponManager.h"
#include "MotionBlur.h"
#include "playerWait.h"
#include "playerWalk.h"
#include "playerAttack1.h"
#include "playerAttack2.h"
#include "playerDamage.h"
#include "playerjustAvoid.h"
#include "PlayerBlowAway.h"
#include "playerJustAvoidAttack1.h"
#include "playerJustAvoidAttack2.h"
#include "playerAttack3.h"
#include "playerAttack4.h"
#include "playerJustAvoidAttack3.h"
#include "playerJustAvoidAttack4.h"
#include "playerJustAvoidAttack5.h"
#include "LoadManager.h"
#include "enemy.h"
#include "EnemyStateManager.h"
#include "enemyAttack1.h"
#include "FadeTransitor.h"
#include "GameManager.h"
#include "PlayerDie.h"
#include "EnemyManager.h"
#include "ComponentManager.h"

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

}

Player::~Player()
{
	//delete playerCom.stateManager;
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
	if (justAvoid) {
		if (playerCom.stateManager->GetState<StateBase>()->GetString() != "PlayerJustAvoid") {
			justAvoid = false;
		}
	}
	//死亡条件
	if (hp <= 0.0f && (playerCom.stateManager->GetState<StateBase>()->GetID() == ID::P_ANIM_IDOL)) {
		playerCom.stateManager->NowChangeState(ID::P_DIE);
		playerCom.stateManager->SetNoStateChange(true);
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
	obj = _obj;
	playerCom.stateManager = obj->Component()->AddComponent<StateManager>();
	

	playerCom.player = this;

	ComponentManager* c = obj->Component();
	
	playerCom.renderer = c->GetComponent<MeshRenderer>();
	playerCom.physics = c->GetComponent<Physics>();

	playerCom.camera = FindGameObject<CameraManager>()->GetCamera()->Component()->GetComponent<Camera>();

	//playerCom.InputManager = FindGameObject<ImputManager>();
	playerCom.controller = FindGameObject<ControllerInputManager>();
	playerCom.keyboard = FindGameObject<KeyboardInputManager>();
	playerCom.InputManager = FindGameObject<InputManager>();

	playerTransform = obj->GetTransform();

	playerCom.anim = obj->Component()->GetComponent<Animator>();
	playerCom.anim->Play(ID::P_ANIM_RUN);
	playerCom.color = obj->Component()->GetComponent<Color>();
	//playerCom.targetObj = FindGameObjectWithTag<Object3D>("ENEMY");
	playerCom.targetObj = nullptr;
	playerCom.hitObj = nullptr;
	playerCom.shaker = c->GetComponent<Shaker>();

	playerCom.effect = FindGameObject<EffectManager>();
	playerCom.sound = FindGameObject<SoundManager>();

	playerCom.weapon = FindGameObject<WeaponManager>();
	playerCom.blur = obj->Component()->GetComponent<MotionBlur>();

	playerCom.gameManager = FindGameObject<GameManager>();
	playerCom.enemyManager = FindGameObject<EnemyManager>();
	
	avoidStart = false;
	justAvoidCanCounter = 0.0f;
	attackColl = nullptr;
	collName = "p_attack";
	//playerCom.physics->SetVelocity(VECTOR3(10.0f, 5.0f, 0.0f));

	//physics->SetInterect(VECTOR3(5.0f, -1.0f, 0.0f),0.1);
	using namespace ID;
	//ステートのセット
	playerCom.stateManager->CreateState<PlayerWait>(GetID(P_ANIM_IDOL));
	playerCom.stateManager->CreateState<PlayerWalk>(GetID(P_ANIM_RUN));
	playerCom.stateManager->CreateState<PlayerAvoid>(GetID(P_ANIM_AVOID));
	playerCom.stateManager->CreateState<PlayerJustAvoid>(GetID(P_ANIM_JUST_AVOID));
	playerCom.stateManager->CreateState<PlayerAttack1>(GetID(P_ANIM_ATTACK1));
	playerCom.stateManager->CreateState<PlayerJustAvoidAttack1>(GetID(P_ANIM_JUST_AVOID_ATTACK1));
	playerCom.stateManager->CreateState<PlayerAttack2>(GetID(P_ANIM_ATTACK2));
	playerCom.stateManager->CreateState<PlayerJustAvoidAttack2>(GetID(P_ANIM_JUST_AVOID_ATTACK2));
	playerCom.stateManager->CreateState<PlayerAttack3>(GetID(P_ANIM_ATTACK3));
	playerCom.stateManager->CreateState<PlayerJustAvoidAttack3>(GetID(P_ANIM_JUST_AVOID_ATTACK3));
	playerCom.stateManager->CreateState<PlayerAttack4>(GetID(P_ANIM_ATTACK4));
	playerCom.stateManager->CreateState<PlayerJustAvoidAttack4>(GetID(P_ANIM_JUST_AVOID_ATTACK4));
	playerCom.stateManager->CreateState<PlayerJustAvoidAttack5>(GetID(P_ANIM_JUST_AVOID_ATTACK5));
	playerCom.stateManager->CreateState<PlayerDamage>(GetID(P_DAMAGE));
	playerCom.stateManager->CreateState<PlayerBlowAway>(GetID(P_BLOWAWAY));
	playerCom.stateManager->CreateState<PlayerDie>(GetID(P_DIE));

	playerCom.stateManager->NodeDrawReady();
	playerCom.stateManager->SetComponent<Player>(this);

	playerCom.stateManager->StartState(P_ANIM_IDOL);
}

void Player::Move(float _speed, float _speedMax)
{
	if (CheckHitKey(KEY_INPUT_RIGHT)) {
		//VECTOR3 power = com.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);
		//if (power.Size() <= 1.0f) {
		playerCom.physics->AddVelocity(VECTOR3(50.0f, 0.0f, 0.0f), false);
		//}

	}
	else if (CheckHitKey(KEY_INPUT_LEFT)) {
		//VECTOR3 power = com.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);
		//if (power.Size() >= -1.0f) {
		playerCom.physics->AddVelocity(VECTOR3(-50.0f, 0.0f, 0.0f), false);
		//}
	}
	else if (CheckHitKey(KEY_INPUT_UP)) {
		playerCom.physics->AddVelocity(VECTOR3(0.0f, 0.0f, 50.0f), false);
	}
	else if (CheckHitKey(KEY_INPUT_DOWN)) {
		playerCom.physics->AddVelocity(VECTOR3(0.0f, 0.0f, -50.0f), false);
	}
	if (CheckHitKey(KEY_INPUT_SPACE)) {
		playerCom.physics->SetVelocity(VGet(0.0f, 300.0f, 0.0f));
	}

	PlayerStickInput();
	std::shared_ptr<PlayerStateBase> pB = playerCom.stateManager->GetState<PlayerStateBase>();
	
	//スティックの傾きの量が少なかったら移動しない
	if ((fabs(walkAngle.x) >= 0.3f || fabs(walkAngle.z) >= 0.3f) && hp > 0.0f) {
		//回転処理
		RotationChange(walkAngle,5.0f);

		VECTOR3 dir = VZero;
		dir.x = walkAngle.x * 1.0f * _speed;
		dir.z = walkAngle.z * 1.0f * _speed;
		dir.y *= 0.0f;
		dir = dir * MGetRotY(playerCom.camera->GetCameraTransform()->rotation.y);
		playerCom.physics->AddVelocity(dir, false);
		VECTOR3 moveVelo;
		moveVelo = playerCom.physics->GetVelocity() * VECTOR3(1.0f, 0.0f, 1.0f);
		
		float max = _speedMax;
		size = moveVelo.SquareSize();
		//最大速度までいったらスピードマックスに補正
		if (moveVelo.SquareSize() >= max * max) {
			moveVelo = moveVelo.Normalize() * _speedMax;
			moveVelo.y = playerCom.physics->GetVelocity().y;
			playerCom.physics->SetVelocity(moveVelo);
		}
		playerCom.stateManager->ChangeState(ID::P_ANIM_RUN);
		//アニメーションのスピードを傾き方で測定
		playerCom.anim->SetPlaySpeed(walkAngle.Size());
	}
	else {
		playerCom.stateManager->ChangeState(ID::P_ANIM_IDOL);
	}
}

void Player::RotationChange(VECTOR3 _angle,float _speed)
{
	VECTOR3 forward = VECTOR3(0, 0, 1) * MGetRotY(playerTransform->rotation.y);
	VECTOR3 right = VECTOR3(1, 0, 0) * MGetRotY(playerTransform->rotation.y);
	VECTOR3 target = _angle * MGetRotY(playerCom.camera->GetCameraTransform()->rotation.y);
	float dot = VDot(target.Normalize(), forward.Normalize());	//コサインの値が正面ベクトルとカメラの角度を計算
	//内積を使って補正
	if (dot >= cosf(_speed * DegToRad)) {
		float inRot = atan2f(target.x, target.z);
		playerTransform->rotation.y = inRot;
		avoidStart = true;
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
	VECTOR dir = VZero;
	dir.x = walkAngle.x * 1.0f * _speed;
	dir.z = walkAngle.z * 1.0f * _speed;
	dir.y *= 0.0f;
	dir = dir * MGetRotY(cameraAngle);
	dir.y += _upSpeed;
	playerCom.physics->SetVelocity(dir);

}

void Player::ImguiDraw()
{
	ImGui::InputFloat("size", &size);
	ImGui::Text(playerCom.stateManager->GetState<StateBase>()->GetString().c_str());
	//playerCom.stateManager->StateNodeDraw();
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth;
	bool open = ImGui::TreeNodeEx("PlayerState", flags);
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
		ImGui::TreePop();
	}

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
	avoidReady = true;
	justAvoidCanCounter = 0.15f;
	avoidCounter--;
	noAvoidCounter = 2.0f;
	avoidReadyCounter = 0.00f;
}

void Player::AvoidRotationChange()
{
	//回避するときにどれくらいの速度で回転させるかを示す処理
	RotationChange(walkAngle,20.0f);
	if (avoidReadyCounter >= 0.0f) {
		avoidReadyCounter -= Time::DeltaTimeRate();
	}
	else {
		justAvoidCanCounter -= Time::DeltaTimeRate();
	}
	if (avoidStart && justAvoidCanCounter <= 0.0f && avoidReadyCounter <= 0.0f) {
		playerCom.stateManager->ChangeState(ID::P_ANIM_AVOID);
		avoidStart = false;
		avoidReady = false;
		justAvoid = false;
		
	}
}

bool Player::EnemyHit(ID::IDType _attackId,BaseObject* _obj)
{
	//敵の攻撃が当たった時の処理
	std::shared_ptr<StateBase> pB = playerCom.stateManager->GetState<StateBase>();
	Animator* enemyAnim = _obj->Component()->GetComponent<Animator>();
	float startTime = enemyAnim->EventStartTime(_attackId);
	bool damage = false;
	//ジャスト回避が出来る処理
	if (justAvoidCanCounter > 0.0f && avoidReadyCounter <= 0.0f) {
		if (enemyAnim->GetCurrentFrame() <= startTime + 2.0f) {
			playerCom.stateManager->ChangeState(ID::P_ANIM_JUST_AVOID);
			playerCom.hitObj = _obj;
			playerCom.enemyManager->JustAvoidTargetChange(dynamic_cast<Object3D*>(_obj));
			playerCom.camera->TargetSet(_obj);
			Debug::DebugLog("JustAvoid");
			avoidStart = false;
			avoidReady = false;
			justAvoidCanCounter = 0.0f;
			justAvoid = true;
		}
	}
	else {
		//ジャスト回避が出来るようになったらスルー
		if (enemyAnim->GetCurrentFrame() <= startTime + 2.0f) {
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
			playerCom.stateManager->ChangeState(ID::P_DAMAGE);
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
	playerCom.targetObj = _base;
}

void Player::HitObjectSet(BaseObject* _base)
{
	playerCom.hitObj = _base;
}

void Player::PlayerAttackHit()
{
	//プレイヤーの攻撃を当てた時のカメラのシェイクヒットストップの処理
	ID::IDType id = playerCom.stateManager->GetState<StateBase>()->GetID();
	switch (id)
	{
	case ID::P_ANIM_ATTACK1:
		playerCom.shaker->ShakeStart(VECTOR3(50, 50, 50), Shaker::HORIZONAL_SHAKE, true, 0.07f);
		playerCom.camera->CameraShake(VECTOR3(40, 40, 40), Shaker::ShakePattern::HORIZONAL_SHAKE, false, 0.1f);
		playerCom.sound->RandamSe("swordHit00000", 7);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK1:
		playerCom.shaker->ShakeStart(VECTOR3(100, 100, 100), Shaker::HORIZONAL_SHAKE, true, 0.15f);
		playerCom.camera->CameraShake(VECTOR3(100, 100, 100), Shaker::ShakePattern::HORIZONAL_SHAKE, false, 0.3f);
		playerCom.sound->RandamSe("swordHit00000", 7);
		break;
	case ID::P_ANIM_ATTACK2:
		playerCom.shaker->ShakeStart(VECTOR3(50, 50, 50), Shaker::HORIZONAL_SHAKE, true, 0.1f);
		playerCom.camera->CameraShake(VECTOR3(40, 40, 40), Shaker::ShakePattern::HORIZONAL_SHAKE, false, 0.1f);
		playerCom.sound->RandamSe("swordHit00000", 7);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK2:
		//playerCom.shaker->ShakeStart(VECTOR3(200, 200, 200), Shaker::HORIZONAL_SHAKE, true, 0.7f);
		//playerCom.camera->CameraShake(VECTOR3(100, 200, 200), Shaker::ShakePattern::HORIZONAL_SHAKE, false, 0.7f);
		
		break;
	case ID::P_ANIM_ATTACK3:
		playerCom.shaker->ShakeStart(VECTOR3(50, 50, 50), Shaker::HORIZONAL_SHAKE, true, 0.12f);
		playerCom.camera->CameraShake(VECTOR3(40, 40, 40), Shaker::ShakePattern::HORIZONAL_SHAKE, false, 0.1f);
		playerCom.sound->RandamSe("swordHit00000", 7);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK3:
		playerCom.shaker->ShakeStart(VECTOR3(150, 100, 100), Shaker::HORIZONAL_SHAKE, true, 0.05f);
		playerCom.camera->CameraShake(VECTOR3(30, 50, 50), Shaker::ShakePattern::HORIZONAL_SHAKE, false, 0.3f);
		playerCom.sound->RandamSe("swordHit00000", 7);
		break;
	case ID::P_ANIM_ATTACK4:
		playerCom.shaker->ShakeStart(VECTOR3(100, 100, 100), Shaker::HORIZONAL_SHAKE, true, 0.2f);
		playerCom.camera->CameraShake(VECTOR3(100, 100, 100), Shaker::ShakePattern::HORIZONAL_SHAKE, false, 0.3f);
		playerCom.sound->RandamSe("swordHit00000", 7);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK4:
		playerCom.shaker->ShakeStart(VECTOR3(100, 100, 100), Shaker::HORIZONAL_SHAKE, true, 0.15f);
		playerCom.camera->CameraShake(VECTOR3(100, 100, 100), Shaker::ShakePattern::HORIZONAL_SHAKE, false, 0.3f);
		playerCom.sound->RandamSe("swordHit00000", 7);
		break;
	case ID::P_ANIM_JUST_AVOID_ATTACK5:
		playerCom.shaker->ShakeStart(VECTOR3(100, 100, 100), Shaker::HORIZONAL_SHAKE, true, 0.3f);
		playerCom.camera->CameraShake(VECTOR3(100, 100, 100), Shaker::ShakePattern::HORIZONAL_SHAKE, false, 0.3f);
		playerCom.sound->RandamSe("swordHit00000", 7);
		break;
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
		playerCom.stateManager->ChangeState(ID::P_ANIM_RUN);
	}
	else {
		playerCom.stateManager->ChangeState(ID::P_ANIM_IDOL);
	}
}

void Player::DeleteCollision()
{
	if (attackColl != nullptr) {
		DeleteColliderComponent();
		attackColl = nullptr;
	}
}

void Player::DrawTrail()
{
	DrawTrail(VECTOR3(0, 0, -70), VECTOR3(0, 0, -150), 255.0f, 0.0f, 0.0f, 100.0f, 28, 0.25f);
	
}

void Player::DrawTrail(VECTOR3 _nPos, VECTOR3 _fPos, float _r, float _g, float _b, float _a, float index, float _time)
{
	//剣の軌跡の処理
	playerCom.weapon->CreateTrailPlayer(_nPos,_fPos,_r,_g,_b,_a,index,_time);
}

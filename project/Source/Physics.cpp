#include "Physics.h"
#include "transform.h"
#include "../ImGui/imgui.h"

namespace {
	const VECTOR3 MIN_FRICTION = VECTOR3(5.0f, 5.0f, 5.0f);
	
	constexpr int PHYSICS_SUBSTEPS = 6;  // 1フレームを何分割するか
	
}

Physics::Physics()
{
	lastTransform = nullptr;
	currentTransform = nullptr;
	gravity = VZero;
	interectAddPos = VZero;
	velocity = VZero;
	frictionCoefficient = 0.0f;
	noGravity = false;
	debugId = 9;
	tag = Function::GetClassNameC<Physics>();
	ground = false;
}

Physics::~Physics()
{
	if (lastTransform != nullptr) {
		delete lastTransform;
	}
}

void Physics::Update()
{
	*lastTransform = *currentTransform;

	float dt = obj->GetObjectTimeRate();

	// 地面にいない場合のみ重力を加算
	if (!ground) {
		velocity += gravity * dt;
	}
	
	// 摩擦減衰
	float resistance = velocity.Size();
	VECTOR3 velo = velocity;
	//velo.y -= 5.0f * dt;

	float newSpeed = max(resistance - firction.Size() * dt, 0.0f);
	velocity = velocity.Normalize() * newSpeed;
	velocity.y = velo.y;

	// 微小速度をカット（誤差防止）
	if (fabs(velocity.y) < 0.0001f)
		velocity.y = 0.0f;

	// 位置更新
	currentTransform->position += velocity * dt;
}

void Physics::Start(VECTOR3 _gravityAmout, VECTOR3 _fir)
{
	currentTransform = obj->GetTransform();
	lastTransform = new Transform(VECTOR3(0,0,0),VZero,VZero);

	gravity = _gravityAmout;
	firction = _fir;
}

void Physics::AddVelocity(VECTOR3 _addVelocity, bool _deltaTime)
{
	VECTOR3 add = _addVelocity;
	if (_deltaTime) {
		add *= obj->GetObjectTimeRate();
	}
	velocity += add;
}

void Physics::AddInterect(VECTOR3 _addInterect, float _firctionCoeffocoent)
{
	interectAddPos += _addInterect;
	frictionCoefficient = _firctionCoeffocoent;
}

void Physics::SetInterect(VECTOR3 _setInterect, float _firctionCoeffocoent)
{
	interectAddPos = _setInterect;
	frictionCoefficient = _firctionCoeffocoent;
}

void Physics::SetFirction(VECTOR3 _setFirction)
{
	firction = _setFirction;
}

void Physics::AddFirction(VECTOR3 _addFirction)
{
	firction += _addFirction;
}

void Physics::SetGravity(VECTOR3 _setGravity)
{
	gravity = _setGravity;
}

void Physics::AddGravity(VECTOR3 _addGraivty)
{
	gravity += _addGraivty;
}

void Physics::ImguiDraw()
{
	ImGui::Separator();
	ImGui::InputFloat3("velocity", &velocity.x);
	ImGui::Separator();
	ImGui::DragFloat3("gravity", &gravity.x, 0.0f, -200.0f, 200.0f);
	ImGui::DragFloat3("firction", &firction.x, 0.0f, -200.0f, 200.0f);
	ImGui::Separator();
	if (ImGui::Button("velocityReset")) {
		velocity = VZero;
	}
	if (ImGui::Button("gravityModeChange")) {
		noGravity = !noGravity;
	}
}

#include "Physics.h"
#include "transform.h"
#include "../ImGui/imgui.h"

namespace {
	const VECTOR3 MIN_FRICTION = VECTOR3(5.0f, 5.0f, 5.0f);
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

	if (!noGravity) {
		velocity += gravity * obj->GetObjectTimeRate(); //重力分velocityに足している
	}
	

	float resis = velocity.Size();
	VECTOR3 velo = velocity;

	float num;

	//抵抗を掛けてベロシティを減らしている
	velo.y -= 5.0f * obj->GetObjectTimeRate();
	num = max(resis - firction.Size() * obj->GetObjectTimeRate(), 0.0f);
	velocity = velocity.Normalize() * num;
	velocity.y = velo.y;
	//ポジションを足す。
	currentTransform->position += velocity * obj->GetObjectTimeRate();
	//今は地面に当たり判定を作っていないので仮置き
	if (currentTransform->position.y <= 0.0f) {
		currentTransform->position.y = 0;
		velocity.y = gravity.y;
	}
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
		add *= Time::DeltaTime();
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

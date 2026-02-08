#include "MeshRenderer2D.h"
#include "../Object/Object3D.h"
#include "../Transform/Transform.h"
#include "../../../ImGui/imgui.h"
#include "../Animator/Anim2D.h"
#include "../ComponentManager.h"
#include "../../Common/Easing.h"

MeshRenderer2D::MeshRenderer2D()
{
	hImage				= -1;
	mode				= DRAW_GRAPH;
	debugId				= 7;
	tag					= Function::GetClassNameC<MeshRenderer2D>();
	transform2D			= nullptr;
	anim2D				= nullptr;
	draw				= true;
	num					= 0;
	addMode				= false;
	feedIn				= false;
	feedOut				= false;
	feedInOutTimer		= 0.0f;
	feedInOutTimerBase	= 0.0f;
}

MeshRenderer2D::~MeshRenderer2D()
{
	if (transform2D != nullptr) {
		delete transform2D;
		transform2D = nullptr;
	}
	/*if (anim2D != nullptr) {
		delete anim2D;
		anim2D = nullptr;
	}*/
}

void MeshRenderer2D::Update()
{
	if (feedIn) {
		alpha = Easing::EasingFlow<float>(&feedInOutTimer, feedInOutTimerBase, 255.0f, 0.0f, Easing::EaseIn<float>);
		if (feedInOutTimer <= 0.0f) {
			feedIn = false;
		}
	}
	else if (feedOut) {
		alpha = Easing::EasingFlow<float>(&feedInOutTimer, feedInOutTimerBase, 0.0f, 255.0f, Easing::EaseIn<float>);
		if (feedInOutTimer <= 0.0f) {
			feedOut = false;
			draw = false;
		}
	}
}

void MeshRenderer2D::Draw()
{
	if (hImage == -1 || !draw) {
		return;
	}
	if (feedIn || feedOut) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)alpha);
	}
	Transform transform;
	if (transform2D == nullptr) {
		transform = *obj->GetTransform();
	}
	else {
		transform = *transform2D;
	}
	if (anim2D != nullptr) {
		int graphNum = anim2D->GetGraphNum();
		int num = (int)anim2D->GetAnimCounter() % graphNum;
		if (xAnim) {
			float de = imageSize.x / (float)graphNum;
			DrawRectRotaGraphFast3F(transform.position.x, transform.position.y, static_cast<int>(de * num), 0, imageSize.x / graphNum, imageSize.y, imageSize.x / graphNum * 0.5f, imageSize.y * 0.5f, transform.scale.x, transform.scale.y, transform.rotation.y, hImage, TRUE);
		}
		else {
			float de = imageSize.y / (float)graphNum;
			DrawRectRotaGraphFast3F(transform.position.x, transform.position.y, 0, static_cast<int>(de * num), imageSize.x, imageSize.y / graphNum, imageSize.x * 0.5f, imageSize.y / graphNum * 0.5f, transform.scale.x, transform.scale.y, transform.rotation.y, hImage, TRUE);
		}
		
	}
	else {
		if (addMode) {
			AddDraw(transform);
		}
		else {
			NormalDraw(transform);
		}
	}
	if (feedIn || feedOut) {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void MeshRenderer2D::TextureHandle(int _image, GraphMode _mode)
{
	
	hImage = _image;
	mode = _mode;

	if (hImage != -1)
	{
		int x(0), y(0);
		GetGraphSize(hImage, &x, &y);
		imageSize.x = x;
		imageSize.y = y;;
		drawImageSize = imageSize;
	}
}

void MeshRenderer2D::SetTransform(const VECTOR3& _pos, const VECTOR3& _rotate, const VECTOR3& _scale)
{
	transform2D = new Transform(_pos, _rotate, _scale);
}

void MeshRenderer2D::SetTransform(const Transform& transform)
{
	transform2D = new Transform(transform);
}

void MeshRenderer2D::ImguiDraw()
{
	if (ImGui::TreeNode("textureTransform") ){
		if (transform2D != nullptr) {
			ImGui::Separator();
			ImGui::DragFloat3("2DPosition", &transform2D->position.x, 0.0f, 1000);
			ImGui::Separator();
			ImGui::DragFloat3("2DRotation", &transform2D->rotation.x, 0.0f, DX_PI_F * 2);
			ImGui::Separator();
			ImGui::DragFloat3("2DScale", &transform2D->scale.x, 1.0f, 10.0f);
			ImGui::Separator();
		}
		if (ImGui::Button("NoDraw")) {
			draw = !draw;
		}
		ImGui::Text("image = %d", hImage);
		ImGui::TreePop();
	}
}

void MeshRenderer2D::SetStartPos(const VECTOR2I& _pos)
{
	startPos = _pos;
}

void MeshRenderer2D::AnimStart(float _speed, int _num, bool _animX)
{
	anim2D = obj->Component()->GetComponent<Anim2D>();
	xAnim = _animX;
}

void MeshRenderer2D::AnimStart(float _speed, int _num)
{
	AnimStart(_speed, _num, true);
}

void MeshRenderer2D::SetDrawImageSize(VECTOR2I _size)
{
	drawImageSize = _size;
}

void MeshRenderer2D::SetPosition(VECTOR3 _position)
{
	if (transform2D == nullptr) {
		obj->GetTransform()->position = _position;
	}
	else {
		transform2D->position = _position;
	}
}

void MeshRenderer2D::NormalDraw(const Transform& transform)
{
	switch (mode)
	{
	case MeshRenderer2D::DRAW_GRAPH:
		DrawGraph((int)transform.position.x, (int)transform.position.y, hImage, true);
		break;
	case MeshRenderer2D::DRAW_RECT_GRAPH:
		DrawRectGraph((int)transform.position.x, (int)transform.position.y, startPos.x, startPos.y, imageSize.x, imageSize.y, hImage, true);
		break;
	case MeshRenderer2D::DRAW_RECT_ROTA_GRAPH:
		DrawRectRotaGraph((int)transform.position.x, (int)transform.position.y, startPos.x, startPos.y, imageSize.x, imageSize.y, 1.0, 0.0, hImage, true);
		break;
	case MeshRenderer2D::DRAW_NUM:
		DrawNum();
		break;
	case MeshRenderer2D::DRAW_RECT_ROTA_GRAPH_FAST_3F:
		DrawRectRotaGraphFast3F(transform.position.x, transform.position.y, startPos.x, startPos.y, drawImageSize.x, drawImageSize.y, imageSize.x * 0.5f, imageSize.y * 0.5f, transform.scale.x, transform.scale.y, transform.rotation.y, hImage, TRUE);
		break;
	case MeshRenderer2D::DRAW_BILLBOARD:
		DrawBillboard3D(transform.position, 0.5f, 0.5f, (float)imageSize.x, 0.0f, hImage,true);
		break;
	}
}

void MeshRenderer2D::AddDraw(const Transform& transform)
{
	/*Transform copy = transform;
	copy.scale = VZero;
	const int LOOP_MAX = 4;
	for (int i = 1; i < LOOP_MAX + 1; i++) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, 65);
		copy.scale = transform.scale / i;
		NormalDraw(copy);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);*/
	NormalDraw(transform);
}

void MeshRenderer2D::FeedInDraw(float _timer)
{
	draw = true;
	feedIn = true;
	feedInOutTimer = _timer;
	feedInOutTimerBase = feedInOutTimer;
}

void MeshRenderer2D::FeedOutDraw(float _timer)
{
	feedOut = true;
	feedInOutTimer = _timer;
	feedInOutTimerBase = feedInOutTimer;
}

void MeshRenderer2D::DrawNum()
{
	Transform transform;
	if (transform2D == nullptr) {
		transform = *obj->GetTransform();
	}
	else {
		transform = *transform2D;
	}
	int tmp = num;
	int dig = 0;
	do
	{
		DrawRectRotaGraph((int)transform.position.x - (int)(imageSize.x / 10 * dig), (int)transform.position.y, tmp % 10 * imageSize.x / 10, 0, imageSize.x / 10, imageSize.y, transform.scale.x, transform.rotation.y, hImage, true, false);
		tmp = tmp / 10;
		dig++;
	} while (tmp > 0);
}


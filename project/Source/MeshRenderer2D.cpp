#include "MeshRenderer2D.h"
#include "Object3D.h"
#include "transform.h"
#include "../ImGui/imgui.h"
#include "Anim2D.h"
#include "ComponentManager.h"

MeshRenderer2D::MeshRenderer2D()
{
	hImage = -1;
	mode = DRAW_GRAPH;
	debugId = 7;
	tag = Function::GetClassNameC<MeshRenderer2D>();
	transform2D = nullptr;
	anim2D = nullptr;
	draw = true;
}

MeshRenderer2D::~MeshRenderer2D()
{
	if (transform2D != nullptr) {
		delete transform2D;
		transform2D = nullptr;
	}
	if (anim2D != nullptr) {
		delete anim2D;
		anim2D = nullptr;
	}
}

void MeshRenderer2D::Update()
{
}

void MeshRenderer2D::Draw()
{
	if (hImage == -1 || !draw) {
		return;
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
		float de = imageSize.x / (float)graphNum;
		DrawRectRotaGraphFast3F(transform.position.x, transform.position.y, static_cast<int>(de * num), 0, imageSize.x / graphNum, imageSize.y, imageSize.x / graphNum * 0.5f, imageSize.y * 0.5f, transform.scale.x, transform.scale.y, transform.rotation.y, hImage, TRUE);
	}
	else {
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
		}
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

void MeshRenderer2D::SetTransform(const VECTOR3 _pos, const VECTOR3 _rotate, const VECTOR3 _scale)
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
		
		ImGui::Text("image = %d", hImage);
		ImGui::TreePop();
	}
}

void MeshRenderer2D::SetStartPos(VECTOR2I _pos)
{
	startPos = _pos;
}

void MeshRenderer2D::AnimStart(float _speed, int _num)
{
	anim2D = obj->Component()->GetComponent<Anim2D>();
}

void MeshRenderer2D::SetDrawImageSize(VECTOR2I _pos)
{
	drawImageSize = _pos;
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
		DrawRectRotaGraph(transform.position.x - (imageSize.x / 10 * dig), transform.position.y, tmp % 10 * imageSize.x / 10, 0, imageSize.x / 10, imageSize.y, transform.scale.x, transform.rotation.y, hImage, true, false);
		tmp = tmp / 10;
		dig++;
	} while (tmp > 0);
}


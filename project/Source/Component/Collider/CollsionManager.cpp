#include "CollsionManager.h"
#include "colliderBase.h"
#include "../../Common/Function.h"
#include "../Transform/Transform.h"
#include "../../Common/Debug/Debug.h"
#include "CollsionEvent.h"
#include "ModelCollider.h"
#include "../ComponentManager.h"
#include "../Physics/Physics.h"
#include "rayCollider.h"
#include "DountCollider.h"
#include "../Shadow/Shadow.h"

CollsionManager::CollsionManager()
{
	using namespace Function;
	using namespace CollsionInformation;
	//当たり判定でどの形同士ならこの関数にいれるという登録
	collsionKind[EnumTag(SPHERE ,SPHERE,SHAPE_MAX)] = &CollsionManager::CollsionSphereToSphere;
	collsionKind[EnumTag(SPHERE ,MODEL,SHAPE_MAX)] = &CollsionManager::CollsionSphereToModel;
	collsionKind[EnumTag(SPHERE, DONUT,SHAPE_MAX)] = &CollsionManager::CollsionSphereToDount;
	collsionKind[EnumTag(MODEL ,RAY,SHAPE_MAX)] = &CollsionManager::CollsionModelToRay;
	collList.clear();

	event = new CollsionEvent();
	InitSetPair();
	//DontDestroyOnSceneChange(true);
}

CollsionManager::~CollsionManager()
{
	delete event;
}

void CollsionManager::Update()
{
	for (auto itr1 = collList.begin(); itr1 != collList.end();itr1++) {
		for (auto itr2 = itr1; itr2 != collList.end(); itr2++) {
			if (itr1 == itr2) {
				continue;
			}
			//当たり判定がどちらかが終了していたら無視する
			if ((*itr1)->GetFinish() || (*itr2)->GetFinish()) {
				continue;
			}
			CollsionInformation::Tag tag1 = (*itr1)->GetCollTag();
			CollsionInformation::Tag tag2 = (*itr2)->GetCollTag();
			//タグ同士の当たり判定に許可がでてなければ無視する。
			if (!CollsionInformation::IsCollPair(tag1, tag2)) {
				continue;
			}

			bool hit = false;
			//当たり判定の形を見て関数ポインタを代入
			if ((*itr1)->GetShape() > (*itr2)->GetShape()) {
				Function::swap(*itr1, *itr2);
			}
			CollsionKind kind = collsionKind[Function::EnumTag((*itr1)->GetShape(), (*itr2)->GetShape(),CollsionInformation::SHAPE_MAX)];
			if (kind == nullptr) {
				continue;
			}
			hit = (this->*kind)((*itr1), (*itr2));
			if (!hit) {
				continue;
			}
			if ((*itr1)->GetOneColl()) {
				(*itr1)->CollsionFinish();
			}
			if ((*itr2)->GetOneColl()) {
				(*itr2)->CollsionFinish();
			}
			event->Event((*itr1), (*itr2));
		}
	}
}

void CollsionManager::Draw()
{
	/*for (ColliderBase* coll : collList) {
		coll->Draw();
	}*/
}

void CollsionManager::AddPush(ColliderBase* obj)
{
	collList.emplace_back(obj);
}

void CollsionManager::RemoveCollList(ColliderBase* obj)
{
	for (auto itr = collList.begin(); itr != collList.end();) {
		if (*itr == obj) {
			collList.erase(itr);
			return;
		}
		else {
			itr++;
		}
	}
}

bool CollsionManager::CollsionSphereToSphere(ColliderBase* col1, ColliderBase* col2)
{
	Transform* trans1 = col1->GetTransform();
	Transform* trans2 = col2->GetTransform();

	float dist = VSize(VSub(trans1->WorldTransform().position, trans2->WorldTransform().position));

	if (dist <= trans1->scale.x + trans2->scale.x) {
		return true;
	}

	return false;
}

bool CollsionManager::CollsionModelToRay(ColliderBase* col1, ColliderBase* col2)
{
	//Transform* trans1 = col1->GetTransform();
	//Transform* trans2 = col2->GetTransform();
	//Transform* trans3 = dynamic_cast<RayCollider*>(col2)->Get2Transform();

	//auto ret = MV1CollCheck_Line(dynamic_cast<ModelCollider*>(col1)->GetModel(), -1, trans2->WorldTransform().position, trans3->WorldTransform().position);
	//VECTOR3 hitPos = ret.HitPosition;
	//if (ret.HitFlag != 0) {
	//	if (trans1->position.Size() == 0.0f || trans1->position.Size() > VSize(ret.HitPosition - trans1->position)) {
	//		VECTOR3 pos  = trans2->WorldTransform().position - hitPos;
	//		col2->GetObj()->Component()->GetComponent<Physics>()->AddVelocity(pos,false);
	//	}
	//	return true;
	//}
	////MV1CollResultPolyDimTerminate(ret);
	//return false;

	Transform* modelTransform = col1->GetTransform(); // 地面など
	Transform* rayStartTrans = col2->GetTransform();  // レイの始点
	Transform* rayEndTrans = dynamic_cast<RayCollider*>(col2)->Get2Transform(); // レイの終点

	// レイの開始点と終了点（ワールド座標）
	VECTOR3 startPos = rayStartTrans->WorldTransform().position;
	VECTOR3 endPos = rayEndTrans->WorldTransform().position;

	// レイによるモデルの当たり判定
	auto result = MV1CollCheck_Line(dynamic_cast<ModelCollider*>(col1)->GetModel(),-1,startPos,endPos
	);
	if (col2->GetCollTag() == CollsionInformation::P_FLOOR) {
		//int a;
	}
	Physics* p = col2->GetObj()->Component()->GetComponent<Physics>();
	Pushback resolver;

	if (result.HitFlag != 0) {
		VECTOR3 push = startPos - result.HitPosition;

		// Y方向のみ押し返し
		resolver.AddPush(VECTOR3(0, 1, 0), push.Size(), CollsionInformation::Shape::RAY);

		if (col2->GetCollTag() == CollsionInformation::SHADOW) {
			col2->GetBaseObject()->Component()->GetComponent<Shadow>()->ChangeScale(push,result.HitPosition);
		}
	}
	resolver.Apply(col2->GetObj()->GetTransform(), p, true, 2.0f * Time::DeltaTimeRate());

	if (p != nullptr) {
		p->SetGround(resolver.IsGrounded(0.7f));
	}
	
	//地面の判定は当たり判定から行う
	
	return true;

}

bool CollsionManager::CollsionSphereToModel(ColliderBase* col1, ColliderBase* col2)
{
	Transform* trans1 = col1->GetTransform();
	Transform* trans2 = col2->GetTransform();



	VECTOR3 pos = trans1->WorldTransform().position + VECTOR3(0,100.0f,0);
	float raius = col1->GetRadius();
	Physics* phy = col1->GetObj()->Component()->GetComponent<Physics>();
	//MV1RefreshCollInfo(dynamic_cast<ModelCollider*>(col1)->GetModel(), -1);
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(dynamic_cast<ModelCollider*>(col2)->GetModel(), -1, pos, raius);
	VECTOR3 ret = VZero;

	Pushback resolver;

	for (int i = 0; i < result.HitNum; i++) {
		auto& pol = result.Dim[i];

		VECTOR3 centerToPoly = pos - pol.Position[i];
		float penetration = VDot(pol.Normal, centerToPoly);

		resolver.AddPush(pol.Normal, penetration,CollsionInformation::SPHERE);
	}

	// 解決
	resolver.Apply(col1->GetObj()->GetTransform(), phy,true,10.0f);


	

	return false;

}

bool CollsionManager::CollsionSphereToDount(ColliderBase* col1, ColliderBase* col2)
{
	Transform* trans1 = col1->GetTransform();
	Transform* trans2 = col2->GetTransform();
	float outRadius = dynamic_cast<DountCollider*>(col2)->GetOutRadius();

	float dist = VSize(VSub(trans1->WorldTransform().position, trans2->WorldTransform().position));

	if (dist <= trans1->scale.x + outRadius) {
		if (dist >= trans1->scale.x + trans2->scale.x) {
			return true;
		}
	}

	return false;
}

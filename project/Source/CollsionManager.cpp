#include "CollsionManager.h"
#include "colliderBase.h"
#include "function.h"
#include "transform.h"
#include "Debug.h"
#include "CollsionEvent.h"
#include "ModelCollider.h"
#include "ComponentManager.h"
#include "Physics.h"

CollsionManager::CollsionManager()
{
	using namespace Function;
	using namespace CollsionInformation;
	//当たり判定でどの形同士ならこの関数にいれるという登録
	collsionKind[EnumTag(SPHERE ,SPHERE,SHAPE_MAX)] = &CollsionManager::CollsionSphereToSphere;
	collsionKind[EnumTag(SPHERE ,MODEL,SHAPE_MAX)] = &CollsionManager::CollsionSphereToModel;
	collsionKind[EnumTag(MODEL ,RAY,SHAPE_MAX)] = &CollsionManager::CollsionSphereToSphere;
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
	return false;
}

bool CollsionManager::CollsionSphereToModel(ColliderBase* col1, ColliderBase* col2)
{
	Transform* trans1 = col1->GetTransform();
	Transform* trans2 = col2->GetTransform();

	VECTOR3 pos = trans2->WorldTransform().position + VECTOR3(0,100.0f,0);
	float raius = col2->GetRadius();
	Physics* phy = col2->GetObj()->Component()->GetComponent<Physics>();
	//MV1RefreshCollInfo(dynamic_cast<ModelCollider*>(col1)->GetModel(), -1);
	MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(dynamic_cast<ModelCollider*>(col1)->GetModel(), -1, pos, raius);
	VECTOR3 ret = VZero;
	
	for (int i = 0; i < result.HitNum; i++) {

		auto& pol = result.Dim[i];
		VECTOR3 v = pos - phy->GetLastTransform()->position;
		//float len = Segment_Triangle_MinLength(phy->GetLastTransform()->position, pos, pol.Position[0], pol.Position[1], pol.Position[2]);
		// vの長さとradiusから、押し返すベクトルの長さを求める
		float pushLen = col2->GetRadius() - v.Size();
		//法線から押し返す方向を取得
		VECTOR3 pushDir = pol.Normal;
		float len = VDot(pushDir, v);
		VECTOR3 newPush = pushDir * len;

		ret += newPush;
		VECTOR3 pushVecNorm = ret.Normalize();
		float pushIn = VDot(pushVecNorm, newPush);
		if (pushIn < ret.Size()) {
			ret += newPush - pushIn * pushVecNorm;
		}
		else {
			ret = newPush;
		}
	}
		// 押し返すベクトルを求める
					// HitPositionからcenterへ向かうベクトルvを作る
	//	VECTOR3 v = pos - pol.HitPosition;
	//	// vの長さとradiusから、押し返すベクトルの長さを求める
	//	float pushLen = raius - v.Size();
	//	// 押し返すベクトルを求める(vの向き×長さ）

	//	VECTOR3 newPush = v.Normalize() * pushLen;
	//	VECTOR3 v1 = ret;
	//	VECTOR3 v2 = newPush;
	//	VECTOR3 v1n = v1.Normalize();
	//	float vLen = VDot(v1n, v2);
	//	v = v1n * vLen;
	//	VECTOR3 vt = v2 - v;
	//	if (v1.Size() > v.Size()) {
	//		ret = v1 + vt;
	//	}
	//	else {
	//		VECTOR3 v2n = v2.Normalize();
	//		vLen = VDot(v1, v2n);
	//		v = v2n * vLen;
	//		vt = v1 - v;
	//		ret = v2 + vt;
	//	}
	//}
	//trans2->position += ret;
	if (result.HitNum > 0) {
		col2->GetObj()->GetTransform()->position -= ret;
		/*VECTOR3 i = ret;
		printfDx("X : %.1f Y : %.1f Z : %.1f\n", i.x,i.y,i.z);*/
		VECTOR3 i = ret.Normalize() * -VDot(ret.Normalize(), phy->GetVelocity());
		//Debug::DebugLog("x : " + std::to_string(i.x) + "y : " + std::to_string(i.y) + "z : " + std::to_string(i.z));
		phy->AddVelocity(i,false);
	}
	
	//pos += ret.Normalize() * -VDot(ret.Normalize(), phy->GetVelocity() * 2.0f);
	//phy->AddVelocity(ret * -1.0f,false);
	MV1CollResultPolyDimTerminate(result);
	return false;
}

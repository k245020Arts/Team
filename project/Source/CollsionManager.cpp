#include "CollsionManager.h"
#include "colliderBase.h"
#include "function.h"
#include "transform.h"
#include "Debug.h"
#include "CollsionEvent.h"
#include "ModelCollider.h"
#include "ComponentManager.h"
#include "Physics.h"
#include "rayCollider.h"

CollsionManager::CollsionManager()
{
	using namespace Function;
	using namespace CollsionInformation;
	//当たり判定でどの形同士ならこの関数にいれるという登録
	collsionKind[EnumTag(SPHERE ,SPHERE,SHAPE_MAX)] = &CollsionManager::CollsionSphereToSphere;
	collsionKind[EnumTag(SPHERE ,MODEL,SHAPE_MAX)] = &CollsionManager::CollsionSphereToModel;
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
	auto result = MV1CollCheck_Line(
		dynamic_cast<ModelCollider*>(col1)->GetModel(),
		-1,
		startPos,
		endPos
	);
	if (col2->GetCollTag() == CollsionInformation::P_FLOOR) {
		int a;
	}
	Physics* p = col2->GetObj()->Component()->GetComponent<Physics>();
	static PushbackResolver resolver; // オブジェクトに1つ保持する

	resolver.Clear(); // 前フレームの情報をクリア

	if (result.HitFlag != 0) {
		VECTOR3 push = startPos - result.HitPosition;

		// Y方向のみ押し返し
		resolver.AddPush(VECTOR3(0, 1, 0), push.Size(), CollsionInformation::Shape::RAY);
	}

	// 最大押し返し長さ 2.0 を DeltaTime に対応
	resolver.Apply(col2->GetObj()->GetTransform(), p, true, 2.0f * Time::DeltaTimeRate());

	// ground 判定は pushes に依存
	p->SetGround(resolver.IsGrounded(0.7f));
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

	PushbackResolver resolver;

	for (int i = 0; i < result.HitNum; i++) {
		auto& pol = result.Dim[i];

		VECTOR3 centerToPoly = pos - pol.Position[i];
		float penetration = VDot(pol.Normal, centerToPoly);

		resolver.AddPush(pol.Normal, penetration,CollsionInformation::SPHERE);
	}

	// 解決
	resolver.Apply(col1->GetObj()->GetTransform(), phy,true,10.0f);
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
	//if (result.HitNum > 0) {
	//	col1->GetObj()->GetTransform()->position -= ret;
	//	/*VECTOR3 i = ret;
	//	printfDx("X : %.1f Y : %.1f Z : %.1f\n", i.x,i.y,i.z);*/
	//	VECTOR3 i = ret.Normalize() * -VDot(ret.Normalize(), phy->GetVelocity());
	//	//Debug::DebugLog("x : " + std::to_string(i.x) + "y : " + std::to_string(i.y) + "z : " + std::to_string(i.z));
	//	phy->AddVelocity(i,false);
	//}
	//
	////pos += ret.Normalize() * -VDot(ret.Normalize(), phy->GetVelocity() * 2.0f);
	////phy->AddVelocity(ret * -1.0f,false);
	//MV1CollResultPolyDimTerminate(result);
	return false;

	//Transform* trans1 = col1->GetTransform();
	//Transform* trans2 = col2->GetTransform();

	//// Sphereの中心位置（プレイヤーの座標から少し上にすることで安定）
	//VECTOR3 spherePos = trans1->WorldTransform().position + VECTOR3(0, 0.1f, 0);
	//float radius = col1->GetRadius();

	//Physics* physics = col1->GetObj()->Component()->GetComponent<Physics>();

	//// モデルとスフィアの当たり判定
	//MV1_COLL_RESULT_POLY_DIM result = MV1CollCheck_Sphere(
	//	dynamic_cast<ModelCollider*>(col2)->GetModel(), -1, spherePos, radius);

	//// 合算用の押し返しベクトル
	//VECTOR3 totalPush = VZero;

	//for (int i = 0; i < result.HitNum; ++i) {
	//	auto& pol = result.Dim[i];

	//	// スフィア中心からポリゴンまでのベクトル
	//	VECTOR3 toPoly = spherePos - pol.Position[0];

	//	// めり込み方向はポリゴン法線
	//	float penetration = radius - VDot(pol.Normal, toPoly);
	//	if (penetration > 0.0f) {
	//		VECTOR3 pushVec = pol.Normal * penetration;
	//		totalPush += pushVec;
	//	}
	//}

	//// 押し返しベクトルが発生しているなら補正
	//if (result.HitNum > 0 && totalPush.SquareSize() != 0.0f) {
	//	// 位置補正（地面のめり込みを防止）
	//	trans1->position += totalPush;

	//	// 速度補正（地面との反発）
	//	VECTOR3 normal = totalPush.Normalize();
	//	float dot = VDot(normal, physics->GetVelocity());

	//	if (dot < 0.0f) {
	//		VECTOR3 reflectVel = normal * -dot;
	//		physics->AddVelocity(reflectVel, false);
	//	}

	//	// メモリ解放
	//	MV1CollResultPolyDimTerminate(result);
	//	return true;
	//}

	//// メモリ解放
	//MV1CollResultPolyDimTerminate(result);
	//return false;
}

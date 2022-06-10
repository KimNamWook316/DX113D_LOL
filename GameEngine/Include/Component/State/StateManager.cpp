
#include "StateManager.h"
#include "State.h"
#include "../ColliderBox3D.h"
#include "../ColliderSphere.h"
#include "../../GameObject/GameObject.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneManager.h"
#include "../../Scene/CollisionSection.h"
#include "../StateComponent.h"
#include "AirBorneState.h"

CStateManager::CStateManager()
{

}

CStateManager::~CStateManager()
{
	auto iter = m_mapState.begin();
	auto iterEnd = m_mapState.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

void CStateManager::Init()
{
	CAirBorneState* Airborne = new CAirBorneState;

	AddState("Airborne", Airborne);
}

void CStateManager::AddState(const std::string& Name, CState* State)
{
	m_mapState.insert(std::make_pair(Name, State));
}

CState* CStateManager::FindState(const std::string& Name)
{
	auto iter = m_mapState.find(Name);

	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}

void CStateManager::CheckAirborneTarget(const NotifyParameter& Param)
{
	// Param.MyPos를 기준으로 검사할 Collision Section을 찾고, 그 Section내에서 Param.Range내에 있는 오브젝트의 StateList에 Airborne State를 넣어준다
	CColliderBox3D* ColliderBox = Param.Object->FindComponentFromType<CColliderBox3D>();

	CColliderSphere* ColliderSphere = nullptr;

	bool ColliderBoxExist = false;
	bool ColliderSphereExist = false;

	if (ColliderBox)
		ColliderBoxExist = true;

	else
	{
		ColliderSphere = Param.Object->FindComponentFromType<CColliderSphere>();
		if (ColliderSphere)
			ColliderSphereExist = true;
	}

	// 2가지의 충돌체 모두 갖고 있지 않는 object면 에어본 처리 X
	if (!ColliderBoxExist && !ColliderSphereExist)
		return;

	CColliderComponent* Collider = nullptr;
	if (ColliderBoxExist)
		Collider = ColliderBox;
	else
		Collider = ColliderSphere;

	// 충돌체가 속한 Section들
	std::vector<int> vecColliderSection = Collider->GetPrevSections();
	size_t Count = vecColliderSection.size();

	std::vector<CGameObject*>	vecAirborneTarget;
	CSceneCollision* Collision = CSceneManager::GetInst()->GetScene()->GetCollision();

	for (size_t i = 0; i < Count; ++i)
	{
		CCollisionSection* Section = Collision->GetCollisionSectionInfo()->vecSection[vecColliderSection[i]];

		size_t Size = Section->GetPrevColliderSize();

		for (size_t i = 0; i < Size; ++i)
		{
			if (Collider == Section->GetPrevCollider(i))
				continue;

			Vector3 CandidatePos = Section->GetPrevCollider(i)->GetWorldPos();
			CGameObject* CandidateObject = Section->GetPrevCollider(i)->GetGameObject();

			// 에어본 적용 대상이다
			if (abs(CandidatePos.Distance(Collider->GetWorldPos())) < Param.Range)
			{
				CStateComponent* StateComp = CandidateObject->FindComponentFromType<CStateComponent>();

				if (StateComp)
					StateComp->AddState("Airborne");
			}
		}
	}

}

void CStateManager::FindRestraintTarget(const NotifyParameter& Param)
{
}

void CStateManager::FindStunTarget(const NotifyParameter& Param)
{
}

void CStateManager::FindKnockBackTarget(const NotifyParameter& Param)
{
}


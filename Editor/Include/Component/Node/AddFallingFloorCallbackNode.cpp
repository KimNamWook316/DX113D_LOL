
#include "AddFallingFloorCallbackNode.h"
#include "Component/BehaviorTree.h"
#include "Component/ColliderBox3D.h"
#include "Component/StateComponent.h"
#include "Component/PaperBurnComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Scene/Navigation3DManager.h"
#include "Component/NavMeshComponent.h"

CAddFallingFloorCallbackNode::CAddFallingFloorCallbackNode()	:
	m_CollisionStart(false),
	m_AccTime(0.f)
{
	SetTypeID(typeid(CAddFallingFloorCallbackNode).hash_code());
}

CAddFallingFloorCallbackNode::CAddFallingFloorCallbackNode(const CAddFallingFloorCallbackNode& Node)	:
	CActionNode(Node)
{
}

CAddFallingFloorCallbackNode::~CAddFallingFloorCallbackNode()
{
}

NodeResult CAddFallingFloorCallbackNode::OnStart(float DeltaTime)
{
	m_CallStart = true;

	m_OriginPos = m_Object->GetWorldPos();

	m_BoxCollider = m_Object->FindComponentFromType<CColliderBox3D>();

	if (m_BoxCollider)
	{
		m_BoxCollider->AddCollisionCallback<CAddFallingFloorCallbackNode>(Collision_State::Begin, this, &CAddFallingFloorCallbackNode::Trigger);
	}

	return NodeResult::Node_True;
}

NodeResult CAddFallingFloorCallbackNode::OnUpdate(float DeltaTime)
{
	if (!m_CollisionStart)
		return NodeResult::Node_True;

	if (m_AccTime > 1.f)
	{
		auto iter = m_BoxCollider->GerPrevCollisionList().begin();
		auto iterEnd = m_BoxCollider->GerPrevCollisionList().end();

		for (; iter != iterEnd; ++iter)
		{
			CNavAgent* Agent = (*iter)->GetGameObject()->FindObjectComponentFromType<CNavAgent>();

			if (Agent)
			{
				Agent->SetApplyNavMesh(false);
			}
		}

		m_Object->AddWorldPos(0.f, -15.f * DeltaTime, 0.f);
	}

	if (m_AccTime > 2.5f)
	{
		m_Object->SetWorldPos(m_OriginPos);
		// PaperBurn 효과 시작 하는 코드, PaperBurn 끝나면 m_AccTime = 0.f, m_CollisionStart = false로 만드는 코드, 플레이어 스폰 위치로 스폰하는 코드 추가
		
		//CPaperBurnComponent* Comp = m_Object->FindObjectComponentFromType<CPaperBurnComponent>();
		//Comp->SetInverse(true);

		////Comp->SetMaterial();
		//Comp->SetFinishCallback<CAddFallingFloorCallbackNode>(this, &CAddFallingFloorCallbackNode::ResetFallingBlock);

		//if (Comp)
		//	Comp->StartPaperBurn();
		CNavigation3DManager* Manager = CSceneManager::GetInst()->GetScene()->GetNavigation3DManager();
		Vector3 Pos = Manager->GetNavMeshData()->GetPlayerSpawnPos();
		CGameObject* Player = CSceneManager::GetInst()->GetScene()->GetPlayerObject();


		Player->SetWorldPos(Pos);
		CNavAgent* Agent = Player->FindObjectComponentFromType<CNavAgent>();

		if (Agent)
			Agent->SetApplyNavMesh(true);

		int Idx = Manager->GetNavMeshData()->GetPlayerSpawnPolyIndex();

		Manager->SetPlayerPolyIndex(Idx);

		// 원래는 PaperBurn 끝나고 EndCallback으로 호출할 함수
		ResetFallingBlock();
	}

	m_AccTime += DeltaTime;


	return NodeResult::Node_True;
}

NodeResult CAddFallingFloorCallbackNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

NodeResult CAddFallingFloorCallbackNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

void CAddFallingFloorCallbackNode::Trigger(const CollisionResult& Result)
{
	if (m_CollisionStart)
		return;

	m_CollisionStart = true;
}

void CAddFallingFloorCallbackNode::ResetFallingBlock()
{
	m_AccTime = 0.f;
	m_CollisionStart = false;
}

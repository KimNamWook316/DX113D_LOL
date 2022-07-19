
#include "CheckDetectRangeNode.h"
#include "../ObjectDataComponent.h"
#include "../MonsterPathFindCollider.h"
#include "Scene/Scene.h"
#include "Component/NavAgent.h"

CCheckDetectRangeNode::CCheckDetectRangeNode() :
	m_DetectRange(0.f)
{
	SetTypeID(typeid(CCheckDetectRangeNode).hash_code());
}

CCheckDetectRangeNode::CCheckDetectRangeNode(const CCheckDetectRangeNode& Node) :
	CConditionNode(Node)
{
}

CCheckDetectRangeNode::~CCheckDetectRangeNode()
{
}

NodeResult CCheckDetectRangeNode::OnStart(float DeltaTime)
{
	CMonsterPathFindCollider* PathComp = m_Object->FindComponentFromType<CMonsterPathFindCollider>();

	if (!PathComp)
		return NodeResult::Node_False;

	CObjectDataComponent* Comp = m_Object->FindObjectComponentFromType<CObjectDataComponent>();
	CNavAgent* Agent = m_Object->FindObjectComponentFromType<CNavAgent>();

	m_DetectRange = Comp->GetDetectRange();
	m_Player = m_Object->GetScene()->GetPlayerObject();

	Vector3 PlayerPos = m_Player->GetWorldPos();
	Vector3 MyPos = m_Object->GetWorldPos();

	if (PathComp->GetPathFindEnable() && PlayerPos.Distance(MyPos) <= m_DetectRange)
	{
		return NodeResult::Node_True;
	}

	// 길찾기해서 목표 지점으로 가는 도중에 PathFindCollider가 플레이어에 충돌해서 PathFindEnable = false라면
	else if (!PathComp->GetPathFindEnable() && !Agent->IsEmptyPathList())
	{
		Agent->ClearPathList();
		Agent->SetPathFindStart(false);

		return NodeResult::Node_False;
	}

	return NodeResult::Node_False;
	
}

NodeResult CCheckDetectRangeNode::OnUpdate(float DeltaTime)
{

	return NodeResult::Node_True;
}

NodeResult CCheckDetectRangeNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CCheckDetectRangeNode::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}

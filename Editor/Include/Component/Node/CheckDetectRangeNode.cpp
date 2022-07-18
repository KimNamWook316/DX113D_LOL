
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
	CObjectDataComponent* Comp = m_Object->FindObjectComponentFromType<CObjectDataComponent>();
	CNavAgent* Agent = m_Object->FindObjectComponentFromType<CNavAgent>();

	if (!Agent->GetPathFindEnable())
		return NodeResult::Node_False;

	m_DetectRange = Comp->GetDetectRange();
	m_Player = m_Object->GetScene()->GetPlayerObject();

	Vector3 PlayerPos = m_Player->GetWorldPos();
	Vector3 MyPos = m_Object->GetWorldPos();

	if (PlayerPos.Distance(MyPos) <= m_DetectRange)
	{
		return NodeResult::Node_True;
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

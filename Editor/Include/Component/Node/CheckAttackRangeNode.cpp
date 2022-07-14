
#include "CheckAttackRangeNode.h"
#include "../ObjectDataComponent.h"
#include "Scene/Scene.h"

CCheckAttackRangeNode::CCheckAttackRangeNode()	:
	m_DetectRange(0.f)
{
	SetTypeID(typeid(CCheckAttackRangeNode).hash_code());
}

CCheckAttackRangeNode::CCheckAttackRangeNode(const CCheckAttackRangeNode& Node) :
	CConditionNode(Node)
{
}

CCheckAttackRangeNode::~CCheckAttackRangeNode()
{
}

NodeResult CCheckAttackRangeNode::OnStart(float DeltaTime)
{
	m_CallStart = true;

	CObjectDataComponent* Comp = m_Object->FindObjectComponentFromType<CObjectDataComponent>();
	m_DetectRange = Comp->GetDetectRange();
	m_Player = m_Object->GetScene()->GetPlayerObject();

	Vector3 PlayerPos = m_Player->GetWorldPos();
	Vector3 MyPos = m_Object->GetWorldPos();

	if (PlayerPos.Distance(MyPos) <= m_DetectRange)
	{
		return NodeResult::Node_True;
	}

	else
	{
		return NodeResult::Node_False;
	}
}

NodeResult CCheckAttackRangeNode::OnUpdate(float DeltaTime)
{

	return NodeResult::Node_True;
}

NodeResult CCheckAttackRangeNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CCheckAttackRangeNode::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}

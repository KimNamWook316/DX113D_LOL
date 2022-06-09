
#include "CheckAttackTarget.h"

CCheckAttackTarget::CCheckAttackTarget()
{
	SetTypeID(typeid(CCheckAttackTarget).hash_code());
}

CCheckAttackTarget::CCheckAttackTarget(const CCheckAttackTarget& Node)	:
	CConditionNode(Node)
{
}

CCheckAttackTarget::~CCheckAttackTarget()
{
}

NodeResult CCheckAttackTarget::OnStart(float DeltaTime)
{
	CGameObject* Target = m_Object->GetAttackTarget();

	if (!Target)
		return NodeResult::Node_False;
	
	Vector3 TargetPos = Target->GetWorldPos();
	Vector3 MyPos = m_Object->GetWorldPos();

	float Dist = TargetPos.Distance(MyPos);

	if (Dist <= 3.f)
		return NodeResult::Node_True;

	return NodeResult::Node_False;
}

NodeResult CCheckAttackTarget::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CCheckAttackTarget::OnEnd(float DeltaTime)
{
	return NodeResult();
}

bool CCheckAttackTarget::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}

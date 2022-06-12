
#include "CheckTurretAttackFrequency.h"

CCheckTurretAttackFrequency::CCheckTurretAttackFrequency() :
	m_AccTime(0.f)
{
	SetTypeID(typeid(CCheckTurretAttackFrequency).hash_code());
}

CCheckTurretAttackFrequency::CCheckTurretAttackFrequency(const CCheckTurretAttackFrequency& Node) :
	CConditionNode(Node)
{
}

CCheckTurretAttackFrequency::~CCheckTurretAttackFrequency()
{
}

NodeResult CCheckTurretAttackFrequency::OnStart(float DeltaTime)
{
	if (!m_Object->GetNormalAttackTarget())
		return NodeResult::Node_True;

	m_AccTime += DeltaTime;

	if (m_AccTime >= 2.f)
	{
		m_AccTime = 0.f;
		return NodeResult::Node_True;
	}

	else
	{
		return NodeResult::Node_False;
	}
}

NodeResult CCheckTurretAttackFrequency::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CCheckTurretAttackFrequency::OnEnd(float DeltaTime)
{
	return NodeResult();
}

NodeResult CCheckTurretAttackFrequency::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}
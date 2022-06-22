
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
	CGameObject* Target = m_Object->GetNormalAttackTarget();

	if (!Target)
		return NodeResult::Node_False;
	
	Vector3 TargetPos = Target->GetWorldPos();
	Vector3 MyPos = m_Object->GetWorldPos();

	float Dist = TargetPos.Distance(MyPos);

	// 나중엔 csv파일로 모든 챔피언이나 미니언, 포탑의 공격 사거리 읽어와서 처리하기
	//int Range = m_Object->GetChampionInfo().NormalAttackRange;

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

NodeResult CCheckAttackTarget::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}

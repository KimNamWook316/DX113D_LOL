#include "KnightFinalAttackCheck.h"

CKnightFinalAttackCheck::CKnightFinalAttackCheck()
{
	SetTypeID(typeid(CKnightFinalAttackCheck).hash_code());
}

CKnightFinalAttackCheck::CKnightFinalAttackCheck(const CKnightFinalAttackCheck& Node)	:
	CConditionNode(Node)
{
}

CKnightFinalAttackCheck::~CKnightFinalAttackCheck()
{
}

NodeResult CKnightFinalAttackCheck::OnStart(float DeltaTime)
{
	return NodeResult();
}

NodeResult CKnightFinalAttackCheck::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CKnightFinalAttackCheck::OnEnd(float DeltaTime)
{
	return NodeResult();
}

#include "BossBettyCloseAttackSequenceCheck.h"
#include "../GameStateComponent.h"
#include "../GameBehaviorTree.h"
#include "../BossBettyDataComponent.h"

CBossBettyCloseAttackSequenceCheck::CBossBettyCloseAttackSequenceCheck()
{
}

CBossBettyCloseAttackSequenceCheck::CBossBettyCloseAttackSequenceCheck(const CBossBettyCloseAttackSequenceCheck& Node)
{
}

CBossBettyCloseAttackSequenceCheck::~CBossBettyCloseAttackSequenceCheck()
{
}

NodeResult CBossBettyCloseAttackSequenceCheck::OnStart(float DeltaTime)
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	bool CloseAngryAttackEnable = Data->IsCloseAngryAttackEnable();

	if (CloseAngryAttackEnable)
	{
		return NodeResult::Node_True;
	}
	else
	{
		return NodeResult::Node_False;
	}
}

NodeResult CBossBettyCloseAttackSequenceCheck::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CBossBettyCloseAttackSequenceCheck::OnEnd(float DeltaTime)
{
	return NodeResult();
}

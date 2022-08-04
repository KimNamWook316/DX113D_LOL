#include "BossBettyHPStateCheck.h"
#include "../GameStateComponent.h"
#include "../GameBehaviorTree.h"
#include "../BossBettyDataComponent.h"

CBossBettyHPStateCheck::CBossBettyHPStateCheck()
{
	SetTypeID(typeid(CBossBettyHPStateCheck).hash_code());
}

CBossBettyHPStateCheck::CBossBettyHPStateCheck(const CBossBettyHPStateCheck& Node)
{
}

CBossBettyHPStateCheck::~CBossBettyHPStateCheck()
{
}

NodeResult CBossBettyHPStateCheck::OnStart(float DeltaTime)
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	BossBettyHPState BettyHPState = Data->GetBettyHPState();

	if (BettyHPState == BossBettyHPState::Below30 ||
		BettyHPState == BossBettyHPState::Below60)
	{
		return NodeResult::Node_True;
	}
	else
	{
		return NodeResult::Node_False;
	}
}

NodeResult CBossBettyHPStateCheck::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CBossBettyHPStateCheck::OnEnd(float DeltaTime)
{
	return NodeResult();
}

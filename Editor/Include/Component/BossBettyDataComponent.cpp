#include "BossBettyDataComponent.h"

CBossBettyDataComponent::CBossBettyDataComponent() :
	m_ThrowFarAttackEnable(false),
	m_CloseAngryAttackEnable(false),
	m_FarAttackType(BossBettyFarAttackType::Spin)
{
}

CBossBettyDataComponent::CBossBettyDataComponent(const CBossBettyDataComponent& com)
{
}

CBossBettyDataComponent::~CBossBettyDataComponent()
{
}

void CBossBettyDataComponent::Start()
{
}

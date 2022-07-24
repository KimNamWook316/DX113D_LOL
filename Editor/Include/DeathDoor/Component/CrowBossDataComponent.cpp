
#include "CrowBossDataComponent.h"
#include "../DataManager.h"
#include "MonsterNavAgent.h"

CCrowBossDataComponent::CCrowBossDataComponent()
{
	SetTypeID<CCrowBossDataComponent>();

	m_ComponentType = Component_Type::ObjectComponent;
}

CCrowBossDataComponent::CCrowBossDataComponent(const CCrowBossDataComponent& com)
{
}

CCrowBossDataComponent::~CCrowBossDataComponent()
{
}

void CCrowBossDataComponent::Start()
{
	CMonsterDataComponent::Start();

	m_Data = CDataManager::GetInst()->GetObjectData("CrowBoss");

	m_MonsterNavAgent->SetMoveSpeed(m_Data.MoveSpeed);
}


#include "LurkerDataComponent.h"
#include "../DataManager.h"
#include "MonsterNavAgent.h"

CLurkerDataComponent::CLurkerDataComponent()
{
	SetTypeID<CLurkerDataComponent>();

	m_ComponentType = Component_Type::ObjectComponent;
}

CLurkerDataComponent::CLurkerDataComponent(const CLurkerDataComponent& com)	:
	CMonsterDataComponent(com)
{
}

CLurkerDataComponent::~CLurkerDataComponent()
{
}

void CLurkerDataComponent::Start()
{
	CMonsterDataComponent::Start();

	m_Data = CDataManager::GetInst()->GetObjectData("Lurker");

	m_MonsterNavAgent->SetMoveSpeed(m_Data.MoveSpeed);
}

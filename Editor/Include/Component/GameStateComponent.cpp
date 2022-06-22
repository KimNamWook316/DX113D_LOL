
#include "GameStateComponent.h"
#include "GameBehaviorTree.h"

CGameStateComponent::CGameStateComponent()
{
	m_ComponentType = Component_Type::ObjectComponent;

	SetTypeID<CGameStateComponent>();

	m_BehaviorTree = new CGameBehaviorTree;
	m_BehaviorTree->SetOwner(this);
}

CGameStateComponent::CGameStateComponent(const CGameStateComponent& com)
{
}

CGameStateComponent::~CGameStateComponent()
{
}


#include "CrowBossDataComponent.h"
#include "../DataManager.h"
#include "MonsterNavAgent.h"
#include "Component/AnimationMeshComponent.h"

CCrowBossDataComponent::CCrowBossDataComponent()	:
	m_ChainAttackCount(0),
	m_StartFlying(false),
	m_InFlying(false),
	m_StartStomp(false),
	m_Phase(-1),
	m_PhasePick(false),
	m_StartJump(false)
{
	SetTypeID<CCrowBossDataComponent>();

	m_ComponentType = Component_Type::ObjectComponent;
}

CCrowBossDataComponent::CCrowBossDataComponent(const CCrowBossDataComponent& com)	:
	CMonsterDataComponent(com)
{
}

CCrowBossDataComponent::~CCrowBossDataComponent()
{
}

void CCrowBossDataComponent::Start()
{
	CMonsterDataComponent::Start();

	m_Data = CDataManager::GetInst()->GetObjectData("CrowBoss");

	if(m_MonsterNavAgent)
		m_MonsterNavAgent->SetMoveSpeed(m_Data.MoveSpeed);

	m_AnimMesh->GetAnimationInstance()->SetEndFunction<CCrowBossDataComponent>("Jump", this, &CCrowBossDataComponent::OnEndCrowBossJump);
}


void CCrowBossDataComponent::OnEndCrowBossJump()
{
	m_AnimMesh->GetAnimationInstance()->ChangeAnimation("Stomp");
	m_StartStomp = true;
}

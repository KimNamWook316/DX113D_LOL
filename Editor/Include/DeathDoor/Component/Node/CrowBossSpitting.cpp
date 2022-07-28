#include "CrowBossSpitting.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "../CrowBossDataComponent.h"
#include "Component/AnimationMeshComponent.h"

CCrowBossSpitting::CCrowBossSpitting()
{
	SetTypeID(typeid(CCrowBossSpitting).hash_code());
}

CCrowBossSpitting::CCrowBossSpitting(const CCrowBossSpitting& Node) :
	CActionNode(Node)
{
}

CCrowBossSpitting::~CCrowBossSpitting()
{
}

NodeResult CCrowBossSpitting::OnStart(float DeltaTime)
{
	CCrowBossDataComponent* Data = dynamic_cast<CCrowBossDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	if (Data->GetHP() <= 0)
	{
		return NodeResult::Node_True;
	}

	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("GuidedBullet");

	m_CallStart = true;
	return NodeResult::Node_True;
}

NodeResult CCrowBossSpitting::OnUpdate(float DeltaTime)
{
	CCrowBossDataComponent* Data = dynamic_cast<CCrowBossDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	if (Data->GetHP() <= 0)
	{
		return NodeResult::Node_True;
	}

	bool End = Data->Spitting(DeltaTime);

	if (End)
	{
		m_Owner->SetCurrentNode(nullptr);
		return NodeResult::Node_True;
	}
	else
		return NodeResult::Node_False;
}

NodeResult CCrowBossSpitting::OnEnd(float DeltaTime)
{
	return NodeResult();
}
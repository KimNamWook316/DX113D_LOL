
#include "DeathNode.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/BehaviorTree.h"	
#include "../ObjectDataComponent.h"
#include "../GameStateComponent.h"

CDeathNode::CDeathNode()
{
	SetTypeID(typeid(CDeathNode).hash_code());
}

CDeathNode::CDeathNode(const CDeathNode& Node)	:
	CActionNode(Node)
{
}

CDeathNode::~CDeathNode()
{
}

NodeResult CDeathNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();
	std::string ObjectName = m_Object->GetName();

	std::string SequenceName = "Death";

	if (m_AnimationMeshComp)
	{
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);
	}

	CObjectDataComponent* Data = dynamic_cast<CObjectDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	Data->SetNoInterrupt(true);
	m_Owner->SetCurrentNode(nullptr);

	return NodeResult::Node_True;
}

NodeResult CDeathNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CDeathNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CDeathNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}


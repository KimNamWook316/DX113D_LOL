
#include "DeathNode.h"
#include "Component/AnimationMeshComponent.h"
#include "Animation/AnimationSequenceInstance.h"
#include "Component/BehaviorTree.h"	

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

	SequenceName = ObjectName + SequenceName;

	if (m_AnimationMeshComp)
	{
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);
	}

	m_Object->SetNoInterrupt(true);


	return NodeResult::Node_True;
}

NodeResult CDeathNode::OnUpdate(float DeltaTime)
{
	m_IsEnd = true;

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


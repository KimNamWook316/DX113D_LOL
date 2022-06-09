
#include "IdleNode.h"
#include "../AnimationMeshComponent.h"
#include "../../Animation/AnimationSequenceInstance.h"
#include "../BehaviorTree.h"

CIdleNode::CIdleNode()
{
	SetTypeID(typeid(CIdleNode).hash_code());
}

CIdleNode::CIdleNode(const CIdleNode& Node)	:
	CActionNode(Node)
{
}

CIdleNode::~CIdleNode()
{
}

NodeResult CIdleNode::OnStart(float DeltaTime)
{
	if (m_Object->IsNavAgentPathListEmpty() && !m_Object->GetAttackTarget())
	{
		m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

		std::string ChampionName = m_Object->GetName();

		std::string SequenceName = ChampionName + "_" + "Idle";

		if (m_AnimationMeshComp)
		{
			m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);
		}

		m_Object->SetNoInterrupt(false);
	}

	return NodeResult::Node_True;
}

NodeResult CIdleNode::OnUpdate(float DeltaTime)
{
	m_IsEnd = true;

	return NodeResult::Node_True;
}

NodeResult CIdleNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

bool CIdleNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

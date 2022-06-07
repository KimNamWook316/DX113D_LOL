
#include "MoveNode.h"
#include "../AnimationMeshComponent.h"
#include "../../Animation/AnimationSequenceInstance.h"
#include "../BehaviorTree.h"

CMoveNode::CMoveNode()
{
	SetTypeID(typeid(CMoveNode).hash_code());
}

CMoveNode::CMoveNode(const CMoveNode& Node)	:
	CActionNode(Node)
{
}

CMoveNode::~CMoveNode()
{
}

NodeResult CMoveNode::OnStart(float DeltaTime)
{
	if (m_Object->IsNavAgentPathListEmpty())
	{
		m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

		std::string ChampionName = m_Object->GetName();

		std::string SequenceName = ChampionName + "_" + "Run";

		if (m_AnimationMeshComp)
		{
			m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);
		}

		m_Object->SetNoInterrupt(false);
	}

	return NodeResult::Node_True;
}

NodeResult CMoveNode::OnUpdate(float DeltaTime)
{
	m_IsEnd = true;

	return NodeResult::Node_True;
}

NodeResult CMoveNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

bool CMoveNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}


#include "SkillENode.h"
#include "../AnimationMeshComponent.h"
#include "../../Animation/AnimationSequenceInstance.h"
#include "../BehaviorTree.h"

CSkillENode::CSkillENode()
{
	SetTypeID(typeid(CSkillENode).hash_code());
}

CSkillENode::CSkillENode(const CSkillENode& Node)
{
}

CSkillENode::~CSkillENode()
{
}

NodeResult CSkillENode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string ChampionName = m_Object->GetName();

	std::string SequenceName = ChampionName + "_" + "SkillE";

	if (m_AnimationMeshComp)
	{
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);
		m_Owner->SetCurrentNode(this);
	}

	return NodeResult::Node_True;
}

NodeResult CSkillENode::OnUpdate(float DeltaTime)
{
	if (m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimEnd())
		m_IsEnd = true;

	return NodeResult::Node_True;
}

NodeResult CSkillENode::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);

	return NodeResult::Node_True;
}

NodeResult CSkillENode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

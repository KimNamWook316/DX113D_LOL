
#include "SkillRNode.h"
#include "../AnimationMeshComponent.h"
#include "../../Animation/AnimationSequenceInstance.h"
#include "../BehaviorTree.h"

CSkillRNode::CSkillRNode()
{
	SetTypeID(typeid(CSkillRNode).hash_code());
}

CSkillRNode::CSkillRNode(const CSkillRNode& Node)
{
}

CSkillRNode::~CSkillRNode()
{
}

NodeResult CSkillRNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string ChampionName = m_Object->GetName();

	std::string SequenceName = ChampionName + "_" + "SkillR";

	if (m_AnimationMeshComp)
	{
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);
		m_Owner->SetCurrentNode(this);
	}

	return NodeResult::Node_True;
}

NodeResult CSkillRNode::OnUpdate(float DeltaTime)
{
	if (m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimEnd())
		m_IsEnd = true;

	return NodeResult::Node_True;
}

NodeResult CSkillRNode::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);

	return NodeResult::Node_True;
}

bool CSkillRNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

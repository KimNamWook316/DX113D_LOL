
#include "SkillWNode.h"
#include "../AnimationMeshComponent.h"
#include "../../Animation/AnimationSequenceInstance.h"
#include "../BehaviorTree.h"

CSkillWNode::CSkillWNode()
{
	SetTypeID(typeid(CSkillWNode).hash_code());
}

CSkillWNode::CSkillWNode(const CSkillWNode& Node)
{
}

CSkillWNode::~CSkillWNode()
{
}

NodeResult CSkillWNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string ChampionName = m_Object->GetName();

	std::string SequenceName = ChampionName + "_" + "SkillW";

	if (m_AnimationMeshComp)
	{
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);
		m_Owner->SetCurrentNode(this);
	}

	return NodeResult::Node_True;
}

NodeResult CSkillWNode::OnUpdate(float DeltaTime)
{
	if (m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimEnd())
		m_IsEnd = true;

	return NodeResult::Node_True;
}

NodeResult CSkillWNode::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);

	return NodeResult::Node_True;
}

NodeResult CSkillWNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

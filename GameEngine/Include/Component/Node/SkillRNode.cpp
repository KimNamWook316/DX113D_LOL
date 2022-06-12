
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
	}

	m_Object->ClearPath();
	m_Object->SetNoInterrupt(true);
	m_CallStart = true;

	return NodeResult::Node_True;
}

NodeResult CSkillRNode::OnUpdate(float DeltaTime)
{
	if (m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimEnd())
	{
		m_Object->SetNoInterrupt(false);
		m_IsEnd = true;
		m_CallStart = false;
		return NodeResult::Node_False;
	}

	else
	{
		m_Owner->SetCurrentNode(this);
		m_Object->SetNoInterrupt(true);
		return NodeResult::Node_Running;
	}
}

NodeResult CSkillRNode::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);
	m_IsEnd = false;

	return NodeResult::Node_True;
}

NodeResult CSkillRNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

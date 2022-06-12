
#include "SkillQNode.h"
#include "../AnimationMeshComponent.h"
#include "../../Animation/AnimationSequenceInstance.h"
#include "../BehaviorTree.h"

CSkillQNode::CSkillQNode()
{
	SetTypeID(typeid(CSkillQNode).hash_code());
}

CSkillQNode::CSkillQNode(const CSkillQNode& Node)
{
}

CSkillQNode::~CSkillQNode()
{
}

NodeResult CSkillQNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string ChampionName = m_Object->GetName();

	std::string SequenceName = ChampionName + "_" + "SkillQ";

	if (m_AnimationMeshComp)
	{
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);
	}

	m_Object->ClearPath();
	m_Object->SetNoInterrupt(true);
	m_CallStart = true;

	return NodeResult::Node_True;
}

NodeResult CSkillQNode::OnUpdate(float DeltaTime)
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

NodeResult CSkillQNode::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);
	m_IsEnd = false;

	return NodeResult::Node_True;
}

NodeResult CSkillQNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

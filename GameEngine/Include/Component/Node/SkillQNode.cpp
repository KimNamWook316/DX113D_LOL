
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
		m_Owner->SetCurrentNode(this);
	}

	return NodeResult::Node_True;
}

NodeResult CSkillQNode::OnUpdate(float DeltaTime)
{
	if (m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimEnd())
		m_IsEnd = true;

	return NodeResult::Node_True;
}

NodeResult CSkillQNode::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);

	return NodeResult::Node_True;
}

bool CSkillQNode::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

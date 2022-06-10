
#include "SkillEndCheckNode.h"
#include "../AnimationMeshComponent.h"

CSkillEndCheckNode::CSkillEndCheckNode()
{
	SetTypeID(typeid(CSkillEndCheckNode).hash_code());
}

CSkillEndCheckNode::CSkillEndCheckNode(const CSkillEndCheckNode& Node)	:
	CConditionNode(Node)
{
}

CSkillEndCheckNode::~CSkillEndCheckNode()
{
}

NodeResult CSkillEndCheckNode::OnStart(float DeltaTime)
{
	if (m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimEnd())
	{

		m_Object->SetNoInterrupt(false);
		return NodeResult::Node_True;
	}

	else
		return NodeResult::Node_False;
}

NodeResult CSkillEndCheckNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CSkillEndCheckNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

NodeResult CSkillEndCheckNode::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}

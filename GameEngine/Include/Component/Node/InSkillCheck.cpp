
#include "InSkillCheck.h"
#include "../AnimationMeshComponent.h"

CInSkillCheck::CInSkillCheck()
{
	SetTypeID(typeid(CInSkillCheck).hash_code());
}

CInSkillCheck::CInSkillCheck(const CInSkillCheck& Node)	:
	CConditionNode(Node)
{
}

CInSkillCheck::~CInSkillCheck()
{
}

NodeResult CInSkillCheck::OnStart(float DeltaTime)
{
	if (m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimEnd())
	{

		m_Object->SetNoInterrupt(false);
		return NodeResult::Node_False;
	}

	else
		return NodeResult::Node_True;
}

NodeResult CInSkillCheck::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CInSkillCheck::OnEnd(float DeltaTime)
{
	return NodeResult();
}

NodeResult CInSkillCheck::Invoke(float DeltaTime)
{
	return CConditionNode::Invoke(DeltaTime);
}

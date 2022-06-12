
#include "NormalAttack.h"
#include "../AnimationMeshComponent.h"
#include "../../Animation/AnimationSequenceInstance.h"
#include "../BehaviorTree.h"

CNormalAttack::CNormalAttack()
{
	SetTypeID(typeid(CNormalAttack).hash_code());
}

CNormalAttack::CNormalAttack(const CNormalAttack& Node)	:
	CActionNode(Node)
{
}

CNormalAttack::~CNormalAttack()
{
}

NodeResult CNormalAttack::OnStart(float DeltaTime)
{

	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string ChampionName = m_Object->GetName();

	std::string SequenceName = ChampionName + "_" + "Attack1";

	if (m_AnimationMeshComp)
	{
		m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation(SequenceName);
	}

	m_Object->SetNoInterrupt(false);
	m_CallStart = true;
	
	// Nav Agent의 PathList를 비운다
	CNavAgent* Agent = m_Object->GetNavAgent();

	if (Agent)
		Agent->ClearPathList();

	return NodeResult::Node_True;
}

NodeResult CNormalAttack::OnUpdate(float DeltaTime)
{
	if (!m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimLoop() && m_AnimationMeshComp->GetAnimationInstance()->IsCurrentAnimEnd())
	{
		m_Object->SetNoInterrupt(false);
		m_IsEnd = true;
		m_CallStart = false;
		return NodeResult::Node_False;
	}

	else
	{
		//m_Owner->SetCurrentNode(this);
		return NodeResult::Node_Running;
	}
}

NodeResult CNormalAttack::OnEnd(float DeltaTime)
{
	m_Owner->SetCurrentNode(nullptr);
	m_IsEnd = false;
	m_Object->SetNormalAttackTarget(nullptr);

	return NodeResult::Node_True;
}

NodeResult CNormalAttack::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}


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
	
	// Nav Agent의 PathList를 비운다
	CNavAgent* Agent = m_Object->GetNavAgent();

	if (Agent)
		Agent->ClearPathList();

	return NodeResult::Node_False;
}

NodeResult CNormalAttack::OnUpdate(float DeltaTime)
{
	m_IsEnd = true;

	return NodeResult::Node_False;
}

NodeResult CNormalAttack::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

bool CNormalAttack::Invoke(float DeltaTime)
{
	return CActionNode::Invoke(DeltaTime);
}

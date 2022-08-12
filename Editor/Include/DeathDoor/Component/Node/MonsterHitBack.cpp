#include "MonsterHitBack.h"
#include "../MonsterDataComponent.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/BehaviorTree.h"	
#include "Component/AnimationMeshComponent.h"	
#include "../MonsterDataComponent.h"
#include "../GameStateComponent.h"

CMonsterHitBack::CMonsterHitBack()
{
	SetTypeID(typeid(CMonsterHitBack).hash_code());
}

CMonsterHitBack::CMonsterHitBack(const CMonsterHitBack& Node)
{
}

CMonsterHitBack::~CMonsterHitBack()
{
}

NodeResult CMonsterHitBack::OnStart(float DeltaTime)
{
	CMonsterDataComponent* Data = dynamic_cast<CMonsterDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	// 1) Slash Left
	std::string AnimName = "HitImplace";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	AnimInst->AddNotify(AnimName, "DisableLookPlayer", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);
	AnimInst->AddNotify(AnimName, "DisableMoveZ", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableMoveZ);

	AnimInst->AddNotify(AnimName, "SetCurrentNodeNull", 16,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::SetCurrentNodeNull);

	AnimInst->AddNotify(AnimName, "EnableMoveZ", 16,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableMoveZ);

	return NodeResult::Node_True;
}

NodeResult CMonsterHitBack::OnUpdate(float DeltaTime)
{
	CMonsterDataComponent* Data = dynamic_cast<CMonsterDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	m_Owner->SetCurrentNode(this);

	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("HitImpact");

	return NodeResult::Node_True;
}

NodeResult CMonsterHitBack::OnEnd(float DeltaTime)
{
	return NodeResult();
}

NodeResult CMonsterHitBack::Invoke(float DeltaTime)
{
	return NodeResult();
}

#include "GruntCommonSpawnNode.h"
#include "../GruntCommonDataComponent.h"
#include "../GameStateComponent.h"
#include "../GameBehaviorTree.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "Component/AnimationMeshComponent.h"

CGruntCommonSpawnNode::CGruntCommonSpawnNode() :
	m_SpawnComplete(false)
{
	SetTypeID(typeid(CGruntCommonSpawnNode).hash_code());
}

CGruntCommonSpawnNode::CGruntCommonSpawnNode(const CGruntCommonSpawnNode& Node)
{
}

CGruntCommonSpawnNode::~CGruntCommonSpawnNode()
{
}

void CGruntCommonSpawnNode::Init()
{
	CGruntCommonDataComponent* Data = dynamic_cast<CGruntCommonDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	// 1) Slash Left
	std::string AnimName = "ZombieSpawn";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// 42 : ¸¶Áö¸· Node
	AnimInst->AddNotify(AnimName, "SetCurrentNodeNull", 42,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::SetCurrentNodeNull);
	AnimInst->AddNotify(AnimName, "SpawnEnableFalse", 42,
		Data, &CGruntCommonDataComponent::DisableSpawnEnable);

}

NodeResult CGruntCommonSpawnNode::OnStart(float DeltaTime)
{
	CGruntCommonDataComponent* Data = dynamic_cast<CGruntCommonDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("ZombieSpawn");

	m_Owner->SetCurrentNode(this);

	if (!m_SpawnComplete)
	{
		Data->OnActivateSpawnParticle();
		m_SpawnComplete = true;
	}

	return NodeResult::Node_True;
}

NodeResult CGruntCommonSpawnNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CGruntCommonSpawnNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

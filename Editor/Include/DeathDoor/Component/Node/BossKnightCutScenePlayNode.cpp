#include "BossKnightCutScenePlayNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/Scene.h"
#include "../KnightDataComponent.h"

CBossKnightCutScenePlayNode::CBossKnightCutScenePlayNode()
{
	SetTypeID(typeid(CBossKnightCutScenePlayNode).hash_code());
}

CBossKnightCutScenePlayNode::CBossKnightCutScenePlayNode(const CBossKnightCutScenePlayNode& Node)	:
	CActionNode(Node)
{
}

CBossKnightCutScenePlayNode::~CBossKnightCutScenePlayNode()
{
}

void CBossKnightCutScenePlayNode::Init()
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	CKnightDataComponent* Data = dynamic_cast<CKnightDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	AnimInst->SetEndFunction("MegaSlam", Data, &CKnightDataComponent::OnEndCutScenePlaying);
}

NodeResult CBossKnightCutScenePlayNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("MegaSlam");

	return NodeResult::Node_True;
}

NodeResult CBossKnightCutScenePlayNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CBossKnightCutScenePlayNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

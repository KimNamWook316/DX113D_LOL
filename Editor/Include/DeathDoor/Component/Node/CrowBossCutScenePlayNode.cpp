
#include "CrowBossCutScenePlayNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"
#include "../CrowBossDataComponent.h"

CCrowBossCutScenePlayNode::CCrowBossCutScenePlayNode()
{
	SetTypeID(typeid(CCrowBossCutScenePlayNode).hash_code());
}

CCrowBossCutScenePlayNode::CCrowBossCutScenePlayNode(const CCrowBossCutScenePlayNode& Node) :
	CActionNode(Node)
{
}

CCrowBossCutScenePlayNode::~CCrowBossCutScenePlayNode()
{
}

void CCrowBossCutScenePlayNode::Init()
{
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	CCrowBossDataComponent* Data = dynamic_cast<CCrowBossDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	//AnimInst->AddNotify("CutScene", "CutSceneStart", 0, (CMonsterDataComponent*)Data, &CMonsterDataComponent::OnStartCutScene);
	AnimInst->AddNotify("CutScene", "CameraShake", 62, this, &CCrowBossCutScenePlayNode::CrowBossCutSceneCameraShake);

	//AnimInst->AddNotify("CutScene", "PlayerEnterZoneFalse", 0, (CMonsterDataComponent*)Data, &CMonsterDataComponent::SetPlayerEnterZoneFalse);
	AnimInst->SetEndFunction("CutScene", (CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEndCutScene);
}

NodeResult CCrowBossCutScenePlayNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("CutScene");

	CCrowBossDataComponent* Data = dynamic_cast<CCrowBossDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	Data->SetPlayerEnterZone(false);
	Data->OnStartCutScene();

	return NodeResult::Node_True;
}

NodeResult CCrowBossCutScenePlayNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CCrowBossCutScenePlayNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

void CCrowBossCutScenePlayNode::CrowBossCutSceneCameraShake()
{
	m_Object->GetScene()->GetCameraManager()->ShakeCamera(1.f, 0.6f);
}

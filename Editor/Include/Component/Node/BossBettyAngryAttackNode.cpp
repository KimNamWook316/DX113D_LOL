#include "BossBettyAngryAttackNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/Scene.h"
#include "../BossBettyDataComponent.h"

CBossBettyAngrylAttackNode::CBossBettyAngrylAttackNode()
{
}

CBossBettyAngrylAttackNode::CBossBettyAngrylAttackNode(const CBossBettyAngrylAttackNode& Node)
{
}

CBossBettyAngrylAttackNode::~CBossBettyAngrylAttackNode()
{
}

void CBossBettyAngrylAttackNode::Init()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	// 1) HPDamageAngry
	std::string AnimName = "FirstSlamAnger";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// 던지기 전까지 Player 방향으로 회전할 수 있도록 한다.
	AnimInst->AddNotifyDeltaTimeFrameRange(AnimName, "OnTracePlayer", 0, 5,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnLookPlayer);

	// Snow Ball Falling
	AnimInst->AddNotify(AnimName, "OnStartFallingSnowBallEffect", 9, 
		this, &CBossBettyAngrylAttackNode::OnBossBettyStartFallingSnowBallEffect);

	// 양쪽
	AnimInst->AddNotify(AnimName, "OnTwoSideCloseAttackEffect", 25, 
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);

	// 왼쪽
	AnimInst->AddNotify(AnimName, "OnLeftAttackDownEffect", 34, 
		Data, &CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect);

	// 오른쪽
	AnimInst->AddNotify(AnimName, "OnRightAttackDownEffect", 43, 
		Data, &CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect);

	// 왼쪽
	AnimInst->AddNotify(AnimName, "OnLeftAttackDownEffect", 52, 
		Data, &CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect);

	// 오른쪽
	AnimInst->AddNotify(AnimName, "OnRightAttackDownEffect", 62,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect);

	// 양쪽
	AnimInst->AddNotify(AnimName, "OnTwoSideCloseAttackEffect", 78,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);

	// 울부짖기
	AnimInst->AddNotifyFrameRange(AnimName, "OnBettyRoar", 100, 120,
		Data, &CBossBettyDataComponent::OnBossBettyRoarEffect);
}

NodeResult CBossBettyAngrylAttackNode::OnStart(float DeltaTime)
{
	m_CallStart = true;

	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	
	// HP State 를 다시 None 으로 
	Data->ResetBettyHPState();

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	AnimInst->ChangeAnimation("FirstSlamAnger");

	return NodeResult::Node_True;
}

NodeResult CBossBettyAngrylAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CBossBettyAngrylAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

void CBossBettyAngrylAttackNode::OnBossBettyStartFallingSnowBallEffect()
{
	// 사방에서 투사체 눈덩이가 떨어지게 한다.
}


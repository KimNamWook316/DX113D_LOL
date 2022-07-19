#include "BossBettyCloseAttackNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/Scene.h"
#include "../BossBettyDataComponent.h"

CBossBettyCloseAttackNode::CBossBettyCloseAttackNode()
{
}

CBossBettyCloseAttackNode::CBossBettyCloseAttackNode(const CBossBettyCloseAttackNode& Node)
{
}

CBossBettyCloseAttackNode::~CBossBettyCloseAttackNode()
{
}

void CBossBettyCloseAttackNode::Init()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	// 1) Slash Left
	std::string AnimName = "SlashLeft";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// Animation 진행 중간 끝까지 Player 방향으로 회전할 수 있도록 한다.
	// AnimInst->AddNotifyDeltaTimeFrameRange(AnimName, "OnTracePlayer", 0, 5,
	// 	(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnLookPlayer);

	AnimInst->AddNotify(AnimName, "OnSlashLeft", 18,
		this, &CBossBettyCloseAttackNode::OnBossBettySlashLeftEffect);

	// 2) Slash Right
	AnimName = "SlashRight";

	AnimInst->AddNotify(AnimName, "OnSlashRight", 18,
		this, &CBossBettyCloseAttackNode::OnBossBettySlashRightEffect);

	// 3) PunchLeft
	AnimName = "PunchLeft";

	AnimInst->AddNotify(AnimName, "OnPunchLeft", 18,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateLeftCloseAttackEffect);

	// 4) PunchLeft
	AnimName = "PunchRight";

	AnimInst->AddNotify(AnimName, "OnPunchRight", 18,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateRightCloseAttackEffect);

	// 5) Two Side Attack
	AnimName = "FirstSlam";

	AnimInst->AddNotify(AnimName, "OnFirstSlam", 20,
		Data, &CBossBettyDataComponent::OnBossBettyGenerateTwoSideCloseAttackEffect);

	// 6) Player 쪽으로 방향 바꾸기 
	AnimName = "BackUpStep";

	AnimInst->AddNotifyDeltaTimeFrameRange(AnimName, "OnTracePlayer", 0, 19,
			(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnLookPlayer);
}

NodeResult CBossBettyCloseAttackNode::OnStart(float DeltaTime)
{
	m_CallStart = true;

	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// 근거리 공격 타입을 체크한다.
	m_CloseAttackType = DeterminBettyCloseAttackType();

	switch (m_CloseAttackType)
	{
	case BossBettyCloseAttackType::PunchLeft :
	{
		AnimInst->ChangeAnimation("PunchLeft"); 
	}
	break;
	case BossBettyCloseAttackType::PunchRight:
	{
		AnimInst->ChangeAnimation("PunchRight"); 
	}
	break;
	case BossBettyCloseAttackType::SlashLeft:
	{
		AnimInst->ChangeAnimation("SlashLeft"); 
	}
	break;
	case BossBettyCloseAttackType::SlashRight:
	{
		AnimInst->ChangeAnimation("SlashRight");
	}
	break;
	case BossBettyCloseAttackType::TwoSideFront:
	{
		AnimInst->ChangeAnimation("FirstSlam"); 
	}
	break;	
	case BossBettyCloseAttackType::NotInCloseRange:
	{
		AnimInst->ChangeAnimation("BackUpStep"); 
	}
	break;
	}

	return NodeResult();
}

NodeResult CBossBettyCloseAttackNode::OnUpdate(float DeltaTime)
{
	return NodeResult();
}

NodeResult CBossBettyCloseAttackNode::OnEnd(float DeltaTime)
{
	return NodeResult();
}

BossBettyCloseAttackType CBossBettyCloseAttackNode::DeterminBettyCloseAttackType()
{
	BossBettyCloseAttackType Type;

	// 전방 방향을 비교한다.

	return BossBettyCloseAttackType();
}

void CBossBettyCloseAttackNode::OnBossBettySlashLeftEffect()
{
	// 왼쪽으로 45 도 회전하기 
}

void CBossBettyCloseAttackNode::OnBossBettySlashRightEffect()
{
	// 오른쪽으로 45도 회전하기 
}

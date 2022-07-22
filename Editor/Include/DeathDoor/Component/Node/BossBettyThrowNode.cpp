#include "BossBettyThrowNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/Scene.h"
#include "../BossBettyDataComponent.h"

CBossBettyThrowNode::CBossBettyThrowNode()
{
}

CBossBettyThrowNode::CBossBettyThrowNode(const CBossBettyThrowNode& Node)
{
}

CBossBettyThrowNode::~CBossBettyThrowNode()
{
}

void CBossBettyThrowNode::Init()
{
	m_CallStart = true;

	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// Animation CallBack
	m_AnimationMeshComp = m_Owner->GetAnimationMeshComp();

	std::string AnimName = "Throw";
	CAnimationSequenceInstance* AnimInst = m_AnimationMeshComp->GetAnimationInstance();

	// 던지기 전까지 Player 방향으로 회전할 수 있도록 한다.
	AnimInst->AddNotify(AnimName, "OnTracePlayer", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);
	AnimInst->AddNotify(AnimName, "OnDisableTracePlayer", 26,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableLookPlayer);

	AnimInst->AddNotify(AnimName, "MakeSnowBallAttackObj", 9, this, &CBossBettyThrowNode::MakeSnowBallAttackObj);
	AnimInst->AddNotify(AnimName, "ThrowSnowBallAttackObj", 27, this, &CBossBettyThrowNode::ThrowSnowBallAttackObj);
}

NodeResult CBossBettyThrowNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("Throw");

	return NodeResult::Node_True;
}

NodeResult CBossBettyThrowNode::OnUpdate(float DeltaTime)
{
	return NodeResult::Node_True;
}

NodeResult CBossBettyThrowNode::OnEnd(float DeltaTime)
{
	return NodeResult::Node_True;
}

// Snow Ball Attack Obj 만들기 -> Socket 형태로 달아놓기
void CBossBettyThrowNode::MakeSnowBallAttackObj()
{
	// 아래와 같은, Component 를 들고 있는, 특정 Object 를 만들어낼 것이다.
	// - Project Tile
	// - Mesh
	// - Particle
	// - Collider Component
	// - 그것을, 가져올 것이다. (해당 정보는 BossBettyDataComponent 에 들고 있게 할 것이다)
}

// Snow Ball Attack Obj 를 Player 방향으로 던지기 
void CBossBettyThrowNode::ThrowSnowBallAttackObj()
{
	// BossBettyDataComponent 로부터, BossThrowBall Object 를 가져오고
	// 거기에 적절한 설정들을 여기에 해줄 것이다.
}

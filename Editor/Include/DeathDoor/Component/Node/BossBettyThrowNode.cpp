#include "BossBettyThrowNode.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "../ProjectileComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/ColliderBox3D.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "ObjectPool.h"
#include "../BossBettyDataComponent.h"

CBossBettyThrowNode::CBossBettyThrowNode()
{
	SetTypeID(typeid(CBossBettyThrowNode).hash_code());
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

	// Start
	// 던지기 전까지 Player 방향으로 회전할 수 있도록 한다.
	AnimInst->AddNotify(AnimName, "OnTracePlayer", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnEnableLookPlayer);

	AnimInst->AddNotify(AnimName, "DisableZMove", 0,
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::OnDisableMoveZ);

	// Middle
	AnimInst->AddNotify(AnimName, "MakeSnowBallAttackObj", 9, this, &CBossBettyThrowNode::MakeSnowBallAttackObj);

	// End
	AnimInst->AddNotify(AnimName, "ThrowSnowBallAttackObj", 26, this, &CBossBettyThrowNode::ThrowSnowBallAttackObj);

	AnimInst->SetEndFunction(AnimName, 
		(CMonsterDataComponent*)Data, &CMonsterDataComponent::SetCurrentNodeNull);
}

NodeResult CBossBettyThrowNode::OnStart(float DeltaTime)
{
	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("Throw");

	m_Owner->SetCurrentNode(this);

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
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// 아래와 같은, Component 를 들고 있는, 특정 Object 를 만들어낼 것이다.
	// - Project Tile
	// - Mesh
	// - Particle
	// - Collider Component
	// - 그것을, 가져올 것이다. (해당 정보는 BossBettyDataComponent 에 들고 있게 할 것이다)
	CScene* CurrentScene = CSceneManager::GetInst()->GetScene();

	CGameObject* ThrowBall = CObjectPool::GetInst()->GetProjectile("BossBettySnowAttack", CurrentScene);

	if (ThrowBall == nullptr)
		return;

	Data->SetBettyThrowBallObject(ThrowBall);

	// Particle Component 를 찾아서, Bazier 이동을 시킨다.
	CParticleComponent* ParticleComp = ThrowBall->FindComponentFromType<CParticleComponent>();

	// Betty 바로 앞에 생성한다.
	Vector3 ZLookDir = m_Object->GetWorldAxis(AXIS::AXIS_Z) * -1.f;
	Vector3 YLookDir = m_Object->GetWorldAxis(AXIS::AXIS_Y);

	const Vector3& InitBallPos = m_Object->GetWorldPos() + ZLookDir * 6.f + YLookDir * 2.f;
	ThrowBall->SetWorldPos(InitBallPos);

	// Bazier 에 따라 이동할 수 있게 세팅한다.
	const Vector3& D2 = m_Object->GetWorldPos() + ZLookDir * 9.f + YLookDir * 8.f;
	const Vector3& D3 = m_Object->GetWorldPos() + ZLookDir * 4.5f + YLookDir * 14.f;
	const Vector3& D4 = m_Object->GetWorldPos() + YLookDir * 11.f;

	ParticleComp->SetParticleMoveSpeed(95.f);
	ParticleComp->SetBazierTargetPos(D2, D3, D4, 100);
	ParticleComp->SetBazierMoveEffect(true);
}

// Snow Ball Attack Obj 를 Player 방향으로 던지기 
void CBossBettyThrowNode::ThrowSnowBallAttackObj()
{
	CBossBettyDataComponent* Data = dynamic_cast<CBossBettyDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());

	// BossBettyDataComponent 로부터, BossThrowBall Object 를 가져오고
	// 거기에 적절한 설정들을 여기에 해줄 것이다.
	CGameObject* ThrowBall = Data->GetBossBettyThrowObject();
	CProjectileComponent* ProjTileComp = ThrowBall->FindComponentFromType<CProjectileComponent>();

	CGameObject* AfterEffectParticle = CObjectPool::GetInst()->GetParticle("BettyAttackAfterEffect", CSceneManager::GetInst()->GetScene());
	AfterEffectParticle->SetLifeSpan(1.f);
	AfterEffectParticle->Enable(false);

	const Vector3& PlayerPos = CSceneManager::GetInst()->GetScene()->GetPlayerObject()->GetWorldPos();

	ProjTileComp->ShootByTargetPos(ThrowBall->GetWorldPos(), 180.f, PlayerPos, AfterEffectParticle);
		
	// Throw Attack Enable 을 다시 False 로 바꿔준다
	Data->SetThrowAttackEnable(false);
}

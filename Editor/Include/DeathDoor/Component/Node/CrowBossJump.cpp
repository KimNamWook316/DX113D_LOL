
#include "CrowBossJump.h"
#include "../GameBehaviorTree.h"
#include "../GameStateComponent.h"
#include "Component/AnimationMeshComponent.h"
#include "../CrowBossDataComponent.h"
#include "../MonsterNavAgent.h"
#include "../ProjectileComponent.h"
#include "Scene/Scene.h"

CCrowBossJump::CCrowBossJump()
{
	SetTypeID(typeid(CCrowBossJump).hash_code());
}

CCrowBossJump::CCrowBossJump(const CCrowBossJump& Node) :
	CActionNode(Node)
{
}

CCrowBossJump::~CCrowBossJump()
{
}

NodeResult CCrowBossJump::OnStart(float DeltaTime)
{
	m_AnimationMeshComp->GetAnimationInstance()->ChangeAnimation("Jump");



	return NodeResult::Node_True;
}

NodeResult CCrowBossJump::OnUpdate(float DeltaTime)
{
	CProjectileComponent* Projectile = m_Object->FindObjectComponentFromType<CProjectileComponent>();

	CCrowBossDataComponent* Data = dynamic_cast<CCrowBossDataComponent*>(dynamic_cast<CGameStateComponent*>(m_Owner->GetOwner())->GetData());
	Data->SetStartJump(true);

	Vector3 CurrentPos = m_Object->GetWorldPos();
	Vector3 PlayerPos = m_Object->GetScene()->GetPlayerObject()->GetWorldPos();

	Vector3 Dir = PlayerPos - CurrentPos;
	Dir.Normalize();
	Dir.y = 0.80f;
	Dir.Normalize();

	Projectile->SetNoDestroy(true);
	Projectile->SetNoUpdate(false);
	Projectile->ShootByGravityTargetPos(CurrentPos, Dir, 80.f, PlayerPos);

	return NodeResult::Node_True;
}

NodeResult CCrowBossJump::OnEnd(float DeltaTime)
{
	return NodeResult();
}

#include "FirePlantDataComponent.h"
#include "../DataManager.h"
#include "Component/AnimationMeshComponent.h"
#include "PlayerDataComponent.h"
#include "ObjectPool.h"
#include "ProjectileComponent.h"
#include "Scene/Scene.h"
#include "MonsterBulletData.h"

CFirePlantDataComponent::CFirePlantDataComponent()
{
	SetTypeID<CFirePlantDataComponent>();

	m_AttackCoolDelayTimeMax = 1.f;
}

CFirePlantDataComponent::CFirePlantDataComponent(const CFirePlantDataComponent& com)	:
	CMonsterDataComponent(com)
{
}

CFirePlantDataComponent::~CFirePlantDataComponent()
{
}

void CFirePlantDataComponent::Start()
{
	CMonsterDataComponent::Start();

	m_Data = CDataManager::GetInst()->GetObjectData("FirePlant");
	m_FirePos = dynamic_cast<CSceneComponent*>(m_Object->FindComponent("FirePos"));

	CAnimationSequenceInstance* AnimInst = m_AnimMesh->GetAnimationInstance();

	AnimInst->AddNotify("Fire", "OnStartFire", 0, this, &CFirePlantDataComponent::OnAttackCoolDisable);
	AnimInst->AddNotify("Fire", "OnFire", 25, this, &CFirePlantDataComponent::OnShootBullet);
	AnimInst->SetEndFunction("Fire", this, &CFirePlantDataComponent::OnAttackCoolEnable);
}

void CFirePlantDataComponent::OnAttackCoolEnable()
{
	SetAttackCoolDelayTimeEnable(true);

	m_LookPlayer = true;
}

void CFirePlantDataComponent::OnAttackCoolDisable()
{
	SetAttackCoolDelayTimeEnable(false);

	m_LookPlayer = false;
}

void CFirePlantDataComponent::OnShootBullet()
{
	CGameObject* Projectile = CObjectPool::GetInst()->GetProjectile("FirePlantBullet", m_Scene);

	CMonsterBulletData* Data = Projectile->FindObjectComponentFromType<CMonsterBulletData>();

	Vector3 PlayerPos = m_PlayerData->GetGameObject()->GetWorldPos();
	Vector3 MyPos = m_FirePos->GetWorldPos();

	Vector3 ToPlayer = PlayerPos - MyPos;
	ToPlayer.Normalize();

	if (Data)
	{
		// TODO : FierPlnat - End Particle
		Data->ShootBulletLifeTime(MyPos, ToPlayer, 10.f, 10.f, nullptr);
	}
}

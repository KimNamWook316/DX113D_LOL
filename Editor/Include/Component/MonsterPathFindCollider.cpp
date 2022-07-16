
#include "MonsterPathFindCollider.h"

CMonsterPathFindCollider::CMonsterPathFindCollider()	:
	m_PathFindEnable(true),
	m_PathFindCoolStart(true),
	m_AccTime(0.f),
	m_PathFindCoolTime(2.f)
{
	SetTypeID<CMonsterPathFindCollider>();
	m_ComponentType = Component_Type::SceneComponent;

	m_LayerName = "Collider";
	m_Render = true;

	AddCollisionCallback<CMonsterPathFindCollider>(Collision_State::Begin, this, &CMonsterPathFindCollider::SetPathFindFalse);
	AddCollisionCallback<CMonsterPathFindCollider>(Collision_State::End, this, &CMonsterPathFindCollider::SetPathFindCoolStart);
}

CMonsterPathFindCollider::CMonsterPathFindCollider(const CMonsterPathFindCollider& com)	:
	CColliderSphere(com)
{
}

CMonsterPathFindCollider::~CMonsterPathFindCollider()
{
}

void CMonsterPathFindCollider::Start()
{
	CColliderSphere::Start();
}

bool CMonsterPathFindCollider::Init()
{
	if (!CColliderSphere::Init())
		return false;

	return true;
}

void CMonsterPathFindCollider::Update(float DeltaTime)
{
	CColliderSphere::Update(DeltaTime);

	if (m_PathFindCoolStart)
	{
		m_AccTime += DeltaTime;

		if (m_AccTime >= m_PathFindCoolTime)
		{
			m_AccTime = 0.f;
			m_PathFindEnable = true;
		}
	}
}

void CMonsterPathFindCollider::PostUpdate(float DeltaTime)
{
	CColliderSphere::PostUpdate(DeltaTime);
}

void CMonsterPathFindCollider::PrevRender()
{
	CColliderSphere::PrevRender();
}

void CMonsterPathFindCollider::Render()
{
	CColliderSphere::Render();
}

void CMonsterPathFindCollider::PostRender()
{
	CColliderSphere::PostRender();
}

CMonsterPathFindCollider* CMonsterPathFindCollider::Clone()
{
	return new CMonsterPathFindCollider(*this);
}

bool CMonsterPathFindCollider::Save(FILE* File)
{
	return CColliderSphere::Save(File);

}

bool CMonsterPathFindCollider::Load(FILE* File)
{
	return CColliderSphere::Load(File);
}

bool CMonsterPathFindCollider::SaveOnly(FILE* File)
{
	return false;
}

bool CMonsterPathFindCollider::LoadOnly(FILE* File)
{
	return false;
}

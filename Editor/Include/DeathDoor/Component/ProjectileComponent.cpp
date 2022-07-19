#include "ProjectileComponent.h"
#include "GameObject/GameObject.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"
#include "Component/ParticleComponent.h"

CProjectileComponent::CProjectileComponent()
{
	SetTypeID<CProjectileComponent>();
}

CProjectileComponent::CProjectileComponent(const CProjectileComponent& com)	:
	CObjectComponent(com)
{
	m_Root = m_Object->GetRootComponent();
	m_Collider = m_Object->FindComponentFromType<CColliderSphere>();

	if (!m_Collider)
	{
		m_Collider = m_Object->FindComponentFromType<CColliderBox3D>();
	}

	// TODO : End Particle String값으로 찾을 지 논의
	m_EndParticle = (CParticleComponent*)m_Object->FindComponent("EndParticle");
}

CProjectileComponent::~CProjectileComponent()
{
}

bool CProjectileComponent::Init()
{
	return true;
}

void CProjectileComponent::Start()
{
	CObjectComponent::Start();

	m_Root = m_Object->GetRootComponent();
	m_Collider = m_Object->FindComponentFromType<CColliderSphere>();

	if (!m_Collider)
	{
		m_Collider = m_Object->FindComponentFromType<CColliderBox3D>();
	}

	// TODO : End Particle String값으로 찾을 지 논의
	m_EndParticle = (CParticleComponent*)m_Object->FindComponent("EndParticle");
}

void CProjectileComponent::Update(float DeltaTime)
{
	if (m_IsShoot)
	{
		float GravityMoveY = 0.f;
		if (m_IsGravity)
		{
		}

		Vector3 Move = Vector3(m_Dir.x * m_Speed * DeltaTime, 
			(m_Dir.y * m_Speed * DeltaTime) + GravityMoveY, 
				m_Dir.z * m_Speed * DeltaTime);

		m_Root->AddWorldPos(Move);
	}
}

void CProjectileComponent::PostUpdate(float DeltaTime)
{
	if (m_IsShoot)
	{
	}
}

void CProjectileComponent::PrevRender()
{
}

void CProjectileComponent::Reset()
{
}

void CProjectileComponent::Render()
{
}

void CProjectileComponent::PostRender()
{
}

CObjectComponent* CProjectileComponent::Clone()
{
	return new CProjectileComponent(*this);
}

bool CProjectileComponent::Save(FILE* File)
{
	CObjectComponent::Save(File);

	return true;
}

bool CProjectileComponent::Load(FILE* File)
{
	CObjectComponent::Load(File);

	return true;
}

bool CProjectileComponent::SaveOnly(FILE* File)
{
	CObjectComponent::SaveOnly(File);

	return true;
}

bool CProjectileComponent::LoadOnly(FILE* File)
{
	CObjectComponent::LoadOnly(File);

	return true;
}

void CProjectileComponent::Shoot(const Vector3& StartPos, const Vector3& Dir, float Speed, const Vector3& TargetPos, bool Gravity)
{
	m_IsShoot = true;
	m_StartPos = StartPos;
	m_Dir = Dir;	
	m_Speed = Speed;
	m_TargetPos = TargetPos;
	m_IsGravity = Gravity;
	m_Dir.Normalize();
}

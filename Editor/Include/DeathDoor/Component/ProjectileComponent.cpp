#include "ProjectileComponent.h"
#include "GameObject/GameObject.h"
#include "Component/ColliderBox3D.h"
#include "Component/ColliderSphere.h"
#include "Component/ParticleComponent.h"

#define FIXED_GRAVITY 9.8f 

CProjectileComponent::CProjectileComponent()	:
	m_NoDestroy(false),
	m_NoUpdate(false)
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

	// TODO : End Particle Pool에서 찾기
	if (com.m_EndParticleObject)
	{
	}
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
}

void CProjectileComponent::Update(float DeltaTime)
{
	if (m_NoUpdate)
		return;

	if (m_IsShoot)
	{
		m_LifeTimer += DeltaTime;

		bool IsDestroy = CheckDestroy();

		if (IsDestroy)
		{
			m_Object->Destroy();
		}

		Vector3 Move;

		if (m_IsGravity)
		{
			Move = Vector3(m_Dir.x * m_VelocityXZ * m_LifeTimer,
				(((m_VelocityY * m_LifeTimer) - ((FIXED_GRAVITY * m_LifeTimer * m_LifeTimer) / 2.f))),
				m_Dir.z * m_VelocityXZ * m_LifeTimer);
			m_Root->SetWorldPos(m_StartPos + Move);
		}
		else
		{
			Move = m_Dir * m_Speed * DeltaTime;
			m_Root->AddWorldPos(Move);
		}
	}
}

void CProjectileComponent::PostUpdate(float DeltaTime)
{
}

void CProjectileComponent::PrevRender()
{
}

void CProjectileComponent::Reset()
{
	CObjectComponent::Reset();

	m_IsShoot = false;
	m_LifeTimer = 0.f;
	m_LifeTime = 0.f;
	m_Speed = 0.f;
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

void CProjectileComponent::ShootByTargetPos(const Vector3& StartPos, float Speed, const Vector3& TargetPos, CGameObject* EndParticleObj)
{
	m_IsShoot = true;
	m_StartPos = StartPos;
	m_Speed = Speed;
	m_TargetPos = TargetPos;
	m_IsGravity = false;
	m_EndParticleObject = EndParticleObj;
	m_Dir = TargetPos - StartPos;
	m_Dir.Normalize();
}

void CProjectileComponent::ShootByLifeTime(const Vector3& StartPos, const Vector3& Dir, float Speed, float LifeTime, CGameObject* EndParticleObj)
{
	m_IsShoot = true;
	m_StartPos = StartPos;
	m_Dir = Dir;	
	m_Speed = Speed;
	m_LifeTime = LifeTime;
	m_IsGravity = false;
	m_EndParticleObject = EndParticleObj;
	m_Dir.Normalize();
}

void CProjectileComponent::ShootByGravityTargetPos(const Vector3& StartPos, const Vector3& XZDir, float Angle, const Vector3& TargetPos, CGameObject* EndParticleObj)
{
	m_IsShoot = true;
	m_IsGravity = true;
	m_StartPos = StartPos;
	m_Dir = XZDir;	
	m_TargetPos = TargetPos;
	m_EndParticleObject = EndParticleObj;
	m_Dir.Normalize();

	Vector3 StartXZ = Vector3(StartPos.x, 0.f, StartPos.z);
	Vector3 TargetXZ = Vector3(TargetPos.x, 0.f, TargetPos.z);
	float DistanceXZ = StartXZ.Distance(TargetXZ);
	float Velocity = sqrtf((DistanceXZ * FIXED_GRAVITY) / sinf(DegreeToRadian(2.f * Angle)));
	m_VelocityXZ = Velocity * cosf(DegreeToRadian(Angle));
	m_VelocityY = Velocity * sinf(DegreeToRadian(Angle));
	m_LifeTime = (2 * Velocity * sinf(DegreeToRadian(Angle))) / FIXED_GRAVITY;
}

bool CProjectileComponent::CheckDestroy()
{
	// LifeTime으로 삭제를 관리하는 경우 ( 중력이 적용된 경우에도 LifeTime으로 관리)
	if (m_IsGravity || m_LifeTime != 0.f)
	{
		if (m_LifeTimer >= m_LifeTime)
		{
			if (m_NoDestroy)
				return false;

			OnEnd();
			return true;
		}
	}
	// Target Position도착 여부로 삭제를 관리하는 경우
	else
	{
		if (m_NoDestroy)
			return false;

		Vector3 MyPos = m_Root->GetWorldPos();
		Vector3 ToTarget = m_TargetPos - MyPos;

		float Dot = ToTarget.Dot(m_Dir);

		if (Dot < 0)
		{
			OnEnd();
			return true;
		}
	}

	return false;
}

void CProjectileComponent::OnEnd()
{
	// End Effect가 있는 경우
	if (m_EndParticleObject)
	{
		m_EndParticleObject->Enable(true);
	}

	// TODO : Projectile Destroy처리 확정된 이후 변경
	m_Object->Destroy();
}

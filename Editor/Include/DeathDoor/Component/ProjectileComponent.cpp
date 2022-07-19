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
	if (m_IsShoot)
	{
		m_LifeTimer += DeltaTime;

		bool IsDestroy = CheckDestroy();

		if (IsDestroy)
		{
			return;
		}

		float GravityMoveY = 0.f;
		if (m_IsGravity)
		{
			m_GravityTimer += DeltaTime;
		}

		Vector3 Move = Vector3(m_Dir.x * m_Speed * DeltaTime, 
			(m_Dir.y * m_Speed * DeltaTime) + GravityMoveY, 
				m_Dir.z * m_Speed * DeltaTime);

		m_Root->AddWorldPos(Move);
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

void CProjectileComponent::Shoot(const Vector3& StartPos, const Vector3& Dir, float Speed, 
		const Vector3& TargetPos, bool Gravity, class CGameObject* EndParticleObj)
{
	m_IsShoot = true;
	m_StartPos = StartPos;
	m_Dir = Dir;	
	m_Speed = Speed;
	m_TargetPos = TargetPos;
	m_IsGravity = Gravity;
	m_EndParticleObject = EndParticleObj;
	m_Dir.Normalize();
}

void CProjectileComponent::Shoot(const Vector3& StartPos, const Vector3& Dir,
		float Speed, float LifeTime, bool Gravity, CGameObject* EndParticleObj)
{
	m_IsShoot = true;
	m_StartPos = StartPos;
	m_Dir = Dir;	
	m_Speed = Speed;
	m_LifeTime = LifeTime;
	m_IsGravity = Gravity;
	m_EndParticleObject = EndParticleObj;
	m_Dir.Normalize();
}

bool CProjectileComponent::CheckDestroy()
{
	// 중력의 영향을 받는 경우 목표 지점보다 높이가 낮아질 경우 Destroy
	if (m_IsGravity)
	{
		float CurY = m_Root->GetWorldPos().y;

		if (CurY < m_TargetPos.y)
		{
			OnEnd();
			return true;
		}
	}
	// 중력의 영향을 받지 않는 경우 
	else
	{
		// LifeTime으로 삭제를 관리하는 경우
		if (m_LifeTime != 0.f)
		{
			if (m_LifeTimer >= m_LifeTime)
			{
				OnEnd();
				return true;
			}
		}
		// TargetPosition이 정해진 경우 TargetPosition보다 멀리 온 경우 파괴
		else
		{
			Vector3 MyPos = m_Root->GetWorldPos();
			Vector3 ToTarget = MyPos - m_TargetPos;

			float Dot = ToTarget.Dot(m_Dir);

			if (Dot < 0)
			{
				OnEnd();
				return true;
			}
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
	Destroy();
}

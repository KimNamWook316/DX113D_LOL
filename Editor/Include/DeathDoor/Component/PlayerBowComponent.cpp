
#include "PlayerBowComponent.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "PlayerDataComponent.h"
#include "ObjectPool.h"
#include "ProjectileComponent.h"

CPlayerBowComponent::CPlayerBowComponent()	:
	m_PlayerData(nullptr)
{
	m_ComponentType = Component_Type::SceneComponent;
	SetTypeID<CPlayerBowComponent>();
}

CPlayerBowComponent::CPlayerBowComponent(const CPlayerBowComponent& com)	:
	CStaticMeshComponent(com)
{
}

CPlayerBowComponent::~CPlayerBowComponent()
{
}

void CPlayerBowComponent::Start()
{
	CStaticMeshComponent::Start();

	if (!m_Mesh)
	{
		const PathInfo* Info = CPathManager::GetInst()->FindPath(MESH_PATH);

		char FullPath[MAX_PATH] = {};
		strcpy_s(FullPath, Info->PathMultibyte);
		strcat_s(FullPath, "BOW.msh");

		m_Scene->GetResource()->LoadMeshFullPathMultibyte(Mesh_Type::Static, "Bow", FullPath);

		SetMesh("Bow");

		SetWorldPos(Vector3(FLT_MAX, FLT_MAX, FLT_MAX));
		SetWorldScale(Vector3(0.f, 0.f, 0.f));
		SetWorldRotation(90.f, 0.f, -180.f);

		m_Render = false;
	}

	m_PlayerData = m_Object->FindObjectComponentFromType<CPlayerDataComponent>();
}

bool CPlayerBowComponent::Init()
{
	if (!CStaticMeshComponent::Init())
		return false;

	return true;
}

void CPlayerBowComponent::Update(float DeltaTime)
{
	CStaticMeshComponent::Update(DeltaTime);
}

void CPlayerBowComponent::PostUpdate(float DeltaTime)
{
	CStaticMeshComponent::PostUpdate(DeltaTime);
}

void CPlayerBowComponent::PrevRender()
{
	CStaticMeshComponent::PrevRender();
}

void CPlayerBowComponent::Render()
{
	CStaticMeshComponent::Render();
}

void CPlayerBowComponent::PostRender()
{
	CStaticMeshComponent::PostRender();
}

CPlayerBowComponent* CPlayerBowComponent::Clone()
{
	return new CPlayerBowComponent(*this);
}

void CPlayerBowComponent::ShowBow(const Vector3& ShootDir)
{
	// 네비메쉬 밖을 피킹할 경우 예외 처리
	if (ShootDir.x == 0.f && ShootDir.y == 0.f && ShootDir.z == 0.f)
	{
		return;
	}

	Vector3 Pos = m_Object->GetWorldPos();
	Vector3 BowPos = Vector3(Pos.x + ShootDir.x, Pos.y + 2.f, Pos.z + ShootDir.z);

	// (1, 0, 0)은 화살의 방향
	float DotProduct = ShootDir.Dot(Vector3(1.f, 0.f, 0.f));
	float YDegree = 0.f;

	if (DotProduct < 0.99f && DotProduct > -0.99f)
	{
		float Degree = RadianToDegree(acosf(DotProduct));
		YDegree = Degree;

		if (ShootDir.Cross(Vector3(1.f, 0.f, 0.f)).y > 0.f)
			YDegree *= -1.f;
	}

	SetWorldPos(BowPos);
	SetWorldScale(Vector3(0.06f, 0.06f, 0.06f));
	SetWorldRotation(90.f, YDegree, -180.f);
	SetEmissiveColor(1.f, 0.f, 0.f, 1.f);

	m_Render = true;
}

void CPlayerBowComponent::ShootArrow(const Vector3& ShootDir)
{
	CGameObject* Arrow = CObjectPool::GetInst()->GetProjectile("PlayerArrow");

	if (!Arrow)
		return;

	CProjectileComponent* Comp = Arrow->FindObjectComponentFromType<CProjectileComponent>();

	Comp->Shoot(GetWorldPos(), ShootDir, 10.f, 5.f);
}

bool CPlayerBowComponent::Save(FILE* File)
{
	return CStaticMeshComponent::Save(File);
}

bool CPlayerBowComponent::Load(FILE* File)
{
	return CStaticMeshComponent::Load(File);
}

bool CPlayerBowComponent::SaveOnly(FILE* File)
{
	return true;
}

bool CPlayerBowComponent::LoadOnly(FILE* File)
{
	return true;
}

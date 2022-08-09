
#include "PlayerBowComponent.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "PlayerDataComponent.h"
#include "ObjectPool.h"
#include "ArrowComponent.h"
#include "Component/ColliderSphere.h"
#include "MonsterDataComponent.h"

CPlayerBowComponent::CPlayerBowComponent()	:
	m_PlayerData(nullptr),
	m_Arrow(nullptr),
	m_Destroy(false)
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

	if (m_Destroy)
	{
		m_Arrow->Destroy();
		m_Destroy = false;
	}
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

	if (DotProduct < 0.999999999f && DotProduct > -0.999999999f)
	{
		float Degree = RadianToDegree(acosf(DotProduct));
		YDegree = Degree;

		if (ShootDir.Cross(Vector3(1.f, 0.f, 0.f)).y > 0.f)
			YDegree *= -1.f;
	}

	else
	{
		if (ShootDir.x > 0.f)
			YDegree = 0.f;
		else
			YDegree = 180.f;
	}

	SetWorldPos(BowPos);
	SetWorldScale(Vector3(0.06f, 0.06f, 0.06f));
	SetWorldRotation(90.f, YDegree, -180.f);
	SetEmissiveColor(1.f, 0.f, 0.f, 1.f);

	m_Render = true;
}

void CPlayerBowComponent::ShootArrow(const Vector3& ShootDir)
{
	Vector3 MyPos = m_Object->GetWorldPos();

	m_Arrow = CObjectPool::GetInst()->GetProjectile("PlayerArrow", m_Scene);

	if (!m_Arrow)
		return;

	m_Arrow->SetWorldPos(MyPos.x + ShootDir.x, MyPos.y + 2.f, MyPos.z + ShootDir.z);

	CArrowComponent* Comp = m_Arrow->FindObjectComponentFromType<CArrowComponent>();

	Vector3 InitialDir = Vector3(0.f, 0.f, 1.f);

	float DotProduct = InitialDir.Dot(ShootDir);
	
	if (DotProduct >= -0.999999f && DotProduct <= 0.999999f)
	{
		float Degree = RadianToDegree(acosf(DotProduct));

		Vector3 CrossVector = InitialDir.Cross(ShootDir);

		if (CrossVector.y < 0.f)
			Degree *= -1.f;

		m_Arrow->SetWorldRotationY(Degree);

	}

	Vector3 ArrowStartPos = m_Arrow->GetWorldPos();
	//Comp->ShootByLifeTime(MyPos, ShootDir, 20.f, 2.5f);
	Comp->ClearCollsionCallBack();
	Comp->ShootByLifeTimeCollision<CPlayerBowComponent>(this, &CPlayerBowComponent::OnCollision, Collision_State::Begin, 
		ArrowStartPos, ShootDir, 60.f, 2.5f);
	Comp->SetDestroy(true);
	
}

void CPlayerBowComponent::HideBow()
{
	m_Render = false;
}

void CPlayerBowComponent::OnCollision(const CollisionResult& Result)
{
	m_Object->GetScene()->GetCameraManager()->ShakeCamera(0.4f, 1.f);

	// OnCollision에서 바로 Destroy하면 CCollisionSection::Collision에서 m_vecCollider의 size가 갑자기 바뀌어서 문제가 되므로
	// m_Destroy = true로 만들어줬다가 PrevRender 함수에서 m_Destroy가 true면 Destroy
	m_Destroy = true;

	CGameObject* DestObject = Result.Dest->GetGameObject();

	CMonsterDataComponent* Data = (CMonsterDataComponent*)DestObject->FindObjectComponent("ObjectData");

	if (DestObject->GetObjectType() == Object_Type::Monster && Data)
	{
		Data->SetIsHit(true);
		Data->DecreaseHP(2);
		Data->SetIsHit(false);
	}

	//m_Arrow->Destroy();
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

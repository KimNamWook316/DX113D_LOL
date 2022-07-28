
#include "CrowBossDataComponent.h"
#include "../DataManager.h"
#include "MonsterNavAgent.h"
#include "Component/AnimationMeshComponent.h"
#include "PathManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderHalfLine.h"

CCrowBossDataComponent::CCrowBossDataComponent()	:
	m_StartFlying(false),
	m_InFlying(false),
	m_StartStomp(false),
	m_Phase(-1),
	m_PhasePick(false),
	m_StartJump(false),
	m_CurrentShootCount(0),
	m_ShootAccTime(0.f),
	m_CurrentHookIndex(0),
	m_ClearHookIndex(0),
	m_HookChainTotal(75),
	m_AfterShoot(false),
	m_SpittingStart(false)
{
	SetTypeID<CCrowBossDataComponent>();


	m_ShootState = CrowBossShootState::Ready;
	m_UnitSize = Vector2(0.6f, 0.6f);

	m_ComponentType = Component_Type::ObjectComponent;
}

CCrowBossDataComponent::CCrowBossDataComponent(const CCrowBossDataComponent& com)	:
	CMonsterDataComponent(com)
{
}

CCrowBossDataComponent::~CCrowBossDataComponent()
{
}

void CCrowBossDataComponent::Start()
{
	CMonsterDataComponent::Start();

	m_Data = CDataManager::GetInst()->GetObjectData("CrowBoss");

	if(m_MonsterNavAgent)
		m_MonsterNavAgent->SetMoveSpeed(m_Data.MoveSpeed);

	m_AnimMesh->GetAnimationInstance()->SetEndFunction<CCrowBossDataComponent>("Jump", this, &CCrowBossDataComponent::OnEndCrowBossJump);

	for (int i = 0; i < 100; ++i)
	{
		m_PhaseQueue.push(4);
		m_PhaseQueue.push(3);
		m_PhaseQueue.push(2);
		m_PhaseQueue.push(2);
	}
}

void CCrowBossDataComponent::Update(float DeltaTime)
{

}


void CCrowBossDataComponent::OnEndCrowBossJump()
{
	m_AnimMesh->GetAnimationInstance()->ChangeAnimation("Stomp");
	m_StartStomp = true;
}

void CCrowBossDataComponent::ShootChain(const Vector3& ShootDir, float DeltaTime)
{
	m_ShootState = CrowBossShootState::Shoot;

	if (m_vecHookChain.size() == 0)
	{
		const PathInfo* Info = CPathManager::GetInst()->FindPath(MESH_PATH);

		char FullPath[MAX_PATH] = {};
		strcpy_s(FullPath, Info->PathMultibyte);
		strcat_s(FullPath, "Chain.msh");

		m_Scene->GetResource()->LoadMeshFullPathMultibyte(Mesh_Type::Static, "HookChain", FullPath);

		for (size_t i = 0; i < m_HookChainTotal; ++i)
		{
			CStaticMeshComponent* HookChain = m_Object->CreateComponentAddChild<CStaticMeshComponent>("HookChain");

			HookChain->SetWorldPos(Vector3(FLT_MAX, FLT_MAX, FLT_MAX));
			HookChain->SetMesh("HookChain");
			HookChain->SetWorldScale(Vector3(0.f, 0.f, 0.f));
			HookChain->SetRender(false);

			m_vecHookChain.push_back(HookChain);
		}
	}

	m_ShootAccTime += DeltaTime;

	if (m_CurrentHookIndex < m_HookChainTotal)
	{
		if (m_ShootAccTime > 0.01f)
		{
		
			// 방향에 맞게 HookChain 회전시켜준다
			Vector3 ChainZAxis = m_vecHookChain[m_CurrentHookIndex]->GetWorldAxis(AXIS::AXIS_Z);
			float Angle = ShootDir.Angle(ChainZAxis);

			Vector3 ObjectPos = m_Object->GetWorldPos();
			ObjectPos.x += ShootDir.x * 4.5f;
			ObjectPos.z += ShootDir.z * 4.5f;

			Vector3 HookPos = Vector3(ObjectPos.x + (m_CurrentHookIndex + 1) * ShootDir.x * m_UnitSize.x, ObjectPos.y + 0.7f,
				ObjectPos.z + (m_CurrentHookIndex + 1) * ShootDir.z * m_UnitSize.y);

			m_vecHookChain[m_CurrentHookIndex]->SetRender(true);
			m_vecHookChain[m_CurrentHookIndex]->SetWorldPos(HookPos);
			m_vecHookChain[m_CurrentHookIndex]->SetWorldScale(Vector3(0.06f, 0.06f, 0.06f));
			m_vecHookChain[m_CurrentHookIndex]->GetMaterial(0)->SetBaseColor(0.34f, 0.34f, 0.34f, 1.f);

			Vector3 CrossVec = ShootDir.Cross(ChainZAxis);

			if (CrossVec.y > 0.f)
				Angle *= -1.f;

			//m_vecHookChain[m_CurrentHookIndex]->SetWorldRotation(0.f, Angle, 0.f);

			if (m_CurrentHookIndex % 2 == 0)
			{
				m_vecHookChain[m_CurrentHookIndex]->SetWorldRotation(0.f, Angle, 45.f);
			}

			else
			{
				m_vecHookChain[m_CurrentHookIndex]->SetWorldRotation(0.f, Angle, -45.f);
			}

			++m_CurrentHookIndex;

			m_ShootAccTime = 0.f;
			m_ShootDir = ShootDir;

			return;
		}
	}

	else if (m_CurrentHookIndex == m_HookChainTotal)
	{
		size_t Count = m_vecHookChain.size();

		for (size_t i = 0; i < Count; ++i)
		{
			m_vecHookChain[i]->GetTransform()->SetFixTransform(true);
		}

		m_ShootState = CrowBossShootState::ShootEnd;
		++m_CurrentShootCount;
	}
}

void CCrowBossDataComponent::Fly(const Vector3& FlyDir, float DeltaTime)
{
	m_ShootState = CrowBossShootState::Fly;

	float FlySpeed = m_Data.FlySpeed;

	m_Object->AddWorldPos(FlyDir.x * FlySpeed * DeltaTime, 0.f, FlyDir.z * FlySpeed * DeltaTime);

	Vector3 CurrentPos = m_Object->GetWorldPos();


	size_t Count = m_vecHookChain.size();
	for (size_t i = m_ClearHookIndex; i < Count; ++i)
	{
		Vector3 ChainPos = m_vecHookChain[i]->GetWorldPos();

		if (Vector3(ChainPos.x, 0.f, ChainPos.z).Distance(Vector3(CurrentPos.x, 0.f, CurrentPos.z)) < 1.f)
		{
			for (size_t j = 0; j <= i; ++j)
			{
				m_vecHookChain[j]->SetRender(false);
				m_vecHookChain[j]->Enable(false);
				m_vecHookChain[j]->SetWorldScale(Vector3(0.f, 0.f, 0.f));
			}

			++m_ClearHookIndex;
		}
	}

	if (m_ClearHookIndex == m_HookChainTotal)
	{
		m_CurrentHookIndex = 0;
		m_ClearHookIndex = 0;
		m_ShootState = CrowBossShootState::Done;

		size_t Count = m_vecHookChain.size();

		for (size_t i = 0; i < Count; ++i)
		{
			m_vecHookChain[i]->GetTransform()->SetFixTransform(false);
			m_vecHookChain[i]->SetWorldRotationY(0.f);
		}
	}

}

void CCrowBossDataComponent::Teleport()
{
	m_ShootState = CrowBossShootState::Teleport;

	CGameObject* Player = m_Scene->GetPlayerObject();
	Vector3 PlayerPos = Player->GetWorldPos();
	Vector3 MyPos = m_Object->GetWorldPos();

	Vector3 TeleportPos;

	Vector3 DiffVec = Vector3(MyPos.x, 0.f, MyPos.z) - Vector3(PlayerPos.x, 0.f, PlayerPos.z);
	float DiffX = abs(PlayerPos.x - MyPos.x);
	float DiffZ = abs(PlayerPos.z - MyPos.z);

	int random = rand() % 360 - 180;

	Matrix rotMat;
	rotMat.Rotation(Vector3(0.f, (float)random, 0.f));

	DiffVec = DiffVec.TransformCoord(rotMat);

	TeleportPos = PlayerPos + DiffVec;


	m_Object->SetWorldPos(TeleportPos);


	PlayerPos = Player->GetWorldPos();
	MyPos = m_Object->GetWorldPos();
	// 플레이어 방향으로 바라보게 하기
	Vector3 Dir = PlayerPos - MyPos;
	Dir.Normalize();
	Vector3 FaceDir = m_MonsterNavAgent->GetCurrentFaceDir();

	float DotProduct = FaceDir.Dot(Dir);
	Vector3 CrossVec = FaceDir.Cross(Dir);

	if (DotProduct < 0.999999f && DotProduct > -0.999999f)
	{
		float Degree = RadianToDegree(acosf(DotProduct));

		if (CrossVec.y > 0.f)
			m_Object->AddWorldRotationY(Degree);
		else
			m_Object->AddWorldRotationY(-Degree);
	}
	else
	{
		// 평행이고 반대 방향
		if (DotProduct < -0.999999f)
		{
			if (CrossVec.y > 0)
				m_Object->AddWorldRotationY(180.f);
			else
				m_Object->AddWorldRotationY(-180.f);
		}

	}
}

void CCrowBossDataComponent::Spitting()
{

}

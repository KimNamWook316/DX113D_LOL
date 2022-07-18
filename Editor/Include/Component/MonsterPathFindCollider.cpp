
#include "MonsterPathFindCollider.h"
#include "Collision/CollisionManager.h"
#include "GameObject/GameObject.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/Navigation3DManager.h"

CMonsterPathFindCollider::CMonsterPathFindCollider()	:
	m_PathFindCoolStart(false),
	m_AccTime(0.f),
	m_PathFindCoolTime(2.f)
{
	SetTypeID<CMonsterPathFindCollider>();
	m_ComponentType = Component_Type::SceneComponent;

	m_LayerName = "Collider";
	m_Render = true;

	m_Profile = CCollisionManager::GetInst()->FindProfile("MonsterPathFind");

	AddCollisionCallback<CMonsterPathFindCollider>(Collision_State::Begin, this, &CMonsterPathFindCollider::SetPathFindCoolEnd);
	AddCollisionCallback<CMonsterPathFindCollider>(Collision_State::End, this, &CMonsterPathFindCollider::SetPathFindCoolStart);

	// 다른 몬스터와 충돌한다면 겹치지 않게 밀어내기
	AddCollisionCallback<CMonsterPathFindCollider>(Collision_State::Stay, this, &CMonsterPathFindCollider::PushEachOther);
}

CMonsterPathFindCollider::CMonsterPathFindCollider(const CMonsterPathFindCollider& com)	:
	CColliderSphere(com)
{
}

CMonsterPathFindCollider::~CMonsterPathFindCollider()
{
}

//void CMonsterPathFindCollider::SetPathFindFalse(const CollisionResult& Result)
//{
//	if (Result.Dest->GetGameObject()->GetObjectType() == Object_Type::Player)
//	{
//		m_PathFindEnable = false;
//		m_PathFindCoolStart = false;
//		m_AccTime = 0.f;
//		m_NavAgent->ClearPathList();
//	}
//}

void CMonsterPathFindCollider::PushEachOther(const CollisionResult& Result)
{
	if (Result.Dest->GetCollisionProfile()->Name == "MonsterPathFind")
	{
		CGameObject* DestObject = Result.Dest->GetGameObject();

		Vector3 SrcPos = m_Object->GetRootComponent()->GetWorldPos();
		Vector3 DestObjectPos = DestObject->GetWorldPos();
		CNavAgent* DestObjNavAgent = DestObject->FindObjectComponentFromType<CNavAgent>();

		if (!m_NavAgent || !DestObjNavAgent)
			return;

		if (!m_NavAgent->IsEmptyPathList() && !DestObjNavAgent->IsEmptyPathList())
		{
			Vector3 DestObjecTargetPos = DestObjNavAgent->GetTargetPos();

			DestObjNavAgent->DeleteTargetPos();
			Vector3 Dir = DestObjectPos - SrcPos;
			Dir.Normalize();
			DestObjNavAgent->AddPath(DestObjectPos + Vector3(Dir.x / 2.f, 0.f, Dir.z / 2.f));
			m_NavAgent->AddPath(SrcPos);


			return;

		}

		// 앞에 있는 몬스터가 플레이어와 충돌해서 멈춰있고, 내가 아직 길찾기가 남은 경우 나의 방향을 조금씩 회전해준다
		else if(!m_NavAgent->IsEmptyPathList())
		{
			std::list<Vector3> PathList = m_NavAgent->GetPathList();
			Vector3 FaceDir = m_NavAgent->GetCurrentFaceDir();

			Vector3 DestPos = PathList.back();

			PathList.pop_back();

			float DeltaTime = CEngine::GetInst()->GetDeltaTime();

			Matrix RotMat;
			RotMat.Rotation(0.f, 10.f, 0.f);

			FaceDir = FaceDir.TransformCoord(RotMat);

			Vector3 NewPos = SrcPos + Vector3(FaceDir.x * 2.f, 0.f, FaceDir.z * 2.f);

			m_NavAgent->AddPath(NewPos);
		}

	}
}

void CMonsterPathFindCollider::Start()
{
	CColliderSphere::Start();

	m_NavAgent = m_Object->FindObjectComponentFromType<CNavAgent>();
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

			m_NavAgent->SetPathFindEnable(true);
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

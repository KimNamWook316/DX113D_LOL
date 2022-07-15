
#include "NavAgent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Navigation3DManager.h"
#include "SceneComponent.h"
#include "../GameObject/GameObject.h"
#include "NavMeshComponent.h"

CNavAgent::CNavAgent()	:
	m_MoveSpeed(0.f),
	m_ApplyNavMesh(true),
	m_OccupyPolygonIndex(0)
{
	SetTypeID<CNavAgent>();
}

CNavAgent::CNavAgent(const CNavAgent& com)	:
	CObjectComponent(com)
{
	m_MoveSpeed = com.m_MoveSpeed;
}

CNavAgent::~CNavAgent()
{
}

void CNavAgent::SetUpdateComponent(CSceneComponent* UpdateComponent)
{
	m_UpdateComponent = UpdateComponent;
}

bool CNavAgent::Move(const Vector3& EndPos)
{
	if (!m_UpdateComponent)
		return false;

	return m_Scene->GetNavigationManager()->FindPath<CNavAgent, CSceneComponent>(this, &CNavAgent::PathResult, 
		m_UpdateComponent, EndPos);
}

bool CNavAgent::MoveOnNavMesh(const Vector3 EndPos)
{
	if (!m_Scene->GetNavigation3DManager()->GetNavMeshData())
		return false;

	m_Object->AddWorldPos(EndPos);

	float Height = 0.f;
	bool Valid = m_Scene->GetNavigation3DManager()->CheckPlayerNavMeshPoly(Height);

	if (Valid)
	{
		Vector3 Pos = m_Object->GetWorldPos();
		m_Object->SetWorldPos(Pos.x, Height, Pos.z);
		return true;
	}

	else
	{
		m_Object->SetWorldPos(m_Object->GetPrevFramePos());
		return false;
	}
}

void CNavAgent::Start()
{
	if (!m_UpdateComponent)
		m_UpdateComponent = m_Object->GetRootComponent();
}

bool CNavAgent::Init()
{
	return true;
}

void CNavAgent::Update(float DeltaTime)
{
	if (m_UpdateComponent)
	{
		float Height = 0.f;
		int PolyIndex = 0;
		if (m_OccupyPolygonIndex == -1)
		{
			// 최초로 자신이 어떤 Polygon을 밟고 있는지 체크
			bool Intersect = CSceneManager::GetInst()->GetScene()->GetNavigation3DManager()->CheckNavMeshPoly(m_UpdateComponent->GetWorldPos(), Height, PolyIndex);

			if (Intersect)
				m_OccupyPolygonIndex = PolyIndex;
		}

		if (!m_PathList.empty())
		{
			Vector3	TargetPos = m_PathList.back();
			Vector3	Pos = m_UpdateComponent->GetWorldPos();

			float	TargetDistance = Pos.Distance(TargetPos);

			Vector3	Dir = TargetPos - Pos;
			Dir.Normalize();

			//Vector3 Rot = m_Object->GetWorldRot();
			//Rot.Normalize();

			//Vector3 Diff = Dir - Rot;

			//if (abs(Diff.y) > 0.01f)
			//{
			//	if (Diff.y > 0.f)
			//	{
			//		m_Object->AddWorldRotationY(10.f * DeltaTime);
			//	}

			//	else
			//	{
			//		m_Object->AddWorldRotationY(-10.f * DeltaTime);
			//	}
			//}

			if (m_MoveSpeed == 0.f)
				m_MoveSpeed = 5.f;

			if (TargetDistance <= 0.5f)
			{
				m_PathList.pop_back();
			}

			m_UpdateComponent->AddWorldPos(Dir * m_MoveSpeed * DeltaTime);

			Vector3 NewPos = m_UpdateComponent->GetWorldPos();
			float Height = 0.f;
			int NewPolyIndex = 0;
			// 움직이고 나서 자신이 어떤 Polygon을 밟고 있는지 업데이트
			bool Intersect = CSceneManager::GetInst()->GetScene()->GetNavigation3DManager()->CheckAdjNavMeshPoly(NewPos, m_OccupyPolygonIndex, Height, NewPolyIndex);

			if(Intersect)
				m_OccupyPolygonIndex = NewPolyIndex;
		}
	}

	// NavAgent가 있는데 ApplyNavMesh가 false -> NavMesh를 적용 받지 않아서 아래로 떨어져야 하는 순간
	if (!m_ApplyNavMesh)
	{
		m_Object->AddWorldPos(0.f, -12.f * DeltaTime, 0.f);
	}
}

void CNavAgent::PostUpdate(float DeltaTime)
{
}

void CNavAgent::PrevRender()
{
}

void CNavAgent::Render()
{
}

void CNavAgent::PostRender()
{
}

CNavAgent* CNavAgent::Clone()
{
	return new CNavAgent(*this);
}

bool CNavAgent::Save(FILE* File)
{
	CComponent::Save(File);

	fwrite(&m_MoveSpeed, sizeof(float), 1, File);

	return true;
}

bool CNavAgent::Load(FILE* File)
{
	CComponent::Load(File);

	fread(&m_MoveSpeed, sizeof(float), 1, File);

	return true;
}

bool CNavAgent::SaveOnly(FILE* File)
{
	CComponent::SaveOnly(File);

	fwrite(&m_MoveSpeed, sizeof(float), 1, File);

	return true;
}

bool CNavAgent::LoadOnly(FILE* File)
{
	CComponent::LoadOnly(File);

	fread(&m_MoveSpeed, sizeof(float), 1, File);

	return true;
}

void CNavAgent::PathResult(const std::list<Vector3>& PathList)
{
	m_PathList.clear();
	m_PathList = PathList;
}

void CNavAgent::FillPathList(const std::list<Vector3>& PathList)
{
	auto iter = PathList.begin();
	auto iterEnd = PathList.end();

	for (; iter != iterEnd; ++iter)
	{
		m_PathList.push_back(*iter);
	}
}

void CNavAgent::AddPath(const Vector3& EndPos)
{
	m_PathList.push_back(EndPos);
}

bool CNavAgent::FindPath(CSceneComponent* OwnerComponent, const Vector3& End)
{
	bool Result = CSceneManager::GetInst()->GetScene()->GetNavigation3DManager()->FindPath<CNavAgent, CSceneComponent>(this, &CNavAgent::FillPathList, OwnerComponent, End);

	return Result;
}

bool CNavAgent::CheckStraightPath(const Vector3& StartPos, const Vector3& EndPos, std::vector<Vector3>& vecPath)
{

	return CSceneManager::GetInst()->GetScene()->GetNavigation3DManager()->GetNavMeshData()->CheckStraightPath(StartPos, EndPos, vecPath);
}

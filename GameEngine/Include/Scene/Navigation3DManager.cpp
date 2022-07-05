
#include "Navigation3DManager.h"
#include "../Component/LandScape.h"
#include "../Component/CameraComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneManager.h"
#include "../Input.h"
#include "../Component/NavMeshComponent.h"
#include "../Resource/Mesh/NavMesh.h"

//#include "DirectXMath.h"
#include <DirectXCollision.h>

CNavigation3DManager::CNavigation3DManager()	:
	m_LandScape(nullptr),
	m_NavMeshComponent(nullptr),
	m_PlayerPolyIndex(-1)
{
}

CNavigation3DManager::~CNavigation3DManager()
{
}

CNavMeshComponent* CNavigation3DManager::GetNavMeshData() const
{
	return m_NavMeshComponent;
}

void CNavigation3DManager::SetNavMeshData(CNavMeshComponent* NavComp)
{
	m_NavMeshComponent = NavComp;
}


void CNavigation3DManager::SetNavData(CLandScape* NavData)
{
	m_NavData = NavData;
}

void CNavigation3DManager::AddNavResult(const NavResultData& NavData)
{
}

void CNavigation3DManager::SetLandScape(CLandScape* LandScape)
{
	m_LandScape = LandScape;
}

CLandScape* CNavigation3DManager::GetLandScape() const
{
	return m_LandScape;
}

float CNavigation3DManager::GetY(const Vector3& Pos)
{
	if (!m_NavData)
		return Pos.y;

	return m_NavData->GetHeight(Pos);
}

bool CNavigation3DManager::CheckPickingPoint(Vector3& OutPos)
{
	if (!m_LandScape)
		return false;

	CCameraComponent* Current = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

	Matrix ProjMat = Current->GetProjMatrix();
	Matrix ViewMat = Current->GetViewMatrix();

	Ray ray = CInput::GetInst()->GetRay(ViewMat);
	Vector3 RayDir = ray.Dir;
	Vector3 RayStartPos = ray.Pos;

	float Step = 0.1f;

	while (true)
	{
		Vector3 Point_I1 = RayStartPos;
		Vector3 Point_I2 = RayStartPos + RayDir * Step;

		float Height1 = m_LandScape->GetHeight(Point_I1);
		float Height2 = m_LandScape->GetHeight(Point_I2);

		Vector3 ResultPos;

		if (m_LandScape->CheckInArea(Point_I1, Point_I2, ResultPos))
		{
			OutPos = ResultPos;
			break;
		}

		// LandScape 밖에 클릭하면 피킹 지점 찾지 않도록 예외 처리
		if (Point_I1.x < m_LandScape->GetMin().x && ray.Dir.x <= 0.f)
			return false;

		if (Point_I1.z < m_LandScape->GetMin().z && ray.Dir.z <= 0.f)
			return false;

		if (Point_I1.x > m_LandScape->GetMax().x && ray.Dir.x >= 0.f)
			return false;

		if (Point_I1.z > m_LandScape->GetMax().z && ray.Dir.z >= 0.f)
			return false;

		else
		{
			RayStartPos = Point_I2;
		}
	}

	return true;
}

bool CNavigation3DManager::CheckPlayerNavMeshPoly()
{
	if (!m_NavMeshComponent)
		return false;

	CGameObject* Player = m_Scene->GetPlayerObject();

	if (!Player)
		return false;

	// 플레이어가 어떤 polygon에 위치할지 초기 설정이 되어있지 않아서 모든 polygon을 돌면서 확인
	if (m_PlayerPolyIndex == -1)
	{
		Vector3 PlayerPos = Player->GetWorldPos();

		size_t Count = m_NavMeshComponent->GetNavMesh()->GetNavMeshPolygonCount();

		for (size_t i = 0; i < Count; ++i)
		{
			Vector3 P1 = m_NavMeshComponent->GetVertexPos(i, 0);
			Vector3 P2 = m_NavMeshComponent->GetVertexPos(i, 1);
			Vector3 P3 = m_NavMeshComponent->GetVertexPos(i, 2);

			Matrix WorldMat = m_NavMeshComponent->GetWorldMatrix();

			P1 = P1.TransformCoord(WorldMat);
			P2 = P2.TransformCoord(WorldMat);
			P3 = P3.TransformCoord(WorldMat);

			XMVECTOR v1 = PlayerPos.Convert();

			XMVECTOR Dir = Vector3(0.f, -1.f, 0.f).Convert();
			XMVECTOR _P1 = P1.Convert();
			XMVECTOR _P2 = P2.Convert();
			XMVECTOR _P3 = P3.Convert();

			float Dist = 0.f;

			bool Intersect = DirectX::TriangleTests::Intersects(v1, Dir, _P1, _P2, _P3, Dist);

			if (Intersect)
			{
				m_PlayerPolyIndex = i;
				return true;
			}

		}

		Vector3 P1 = m_NavMeshComponent->GetVertexPos(0, 0);
		Vector3 P2 = m_NavMeshComponent->GetVertexPos(0, 1);
		Vector3 P3 = m_NavMeshComponent->GetVertexPos(0, 2);

		Matrix WorldMat = m_NavMeshComponent->GetWorldMatrix();

		P1 = P1.TransformCoord(WorldMat);
		P2 = P2.TransformCoord(WorldMat);
		P3 = P3.TransformCoord(WorldMat);

		Player->SetWorldPos(Vector3((P1.x + P2.x + P3.x) / 3.f, (P1.y + P2.y + P3.y) / 3.f, (P1.z + P2.z + P3.z) / 3.f));
		m_PlayerPolyIndex = 0;
	}

	// 플레이어가 어떤 polygon에 위치할지 이미 설정이 되어있는 상태라서 지금 Polygon에 존재하는지 체크,
	// 지금 Polygon에 존재하지 않는다면 다른 polygon으로 건너갔다고 판단
	else
	{
		std::vector<Vector3> vecPos;
		m_NavMeshComponent->GetNavPolgonVertexPos(m_PlayerPolyIndex, vecPos);

		Matrix WorldMat = m_NavMeshComponent->GetWorldMatrix();

		Vector3 P1 = vecPos[0];
		Vector3 P2 = vecPos[1];
		Vector3 P3 = vecPos[2];

		P1 = P1.TransformCoord(WorldMat);
		P2 = P2.TransformCoord(WorldMat);
		P3 = P3.TransformCoord(WorldMat);

		CGameObject* Player = m_Scene->GetPlayerObject();
		Vector3 PlayerPos = Player->GetWorldPos();

		XMVECTOR v1 = PlayerPos.Convert();

		XMVECTOR Dir = Vector3(0.f, -1.f, 0.f).Convert();
		XMVECTOR _P1 = P1.Convert();
		XMVECTOR _P2 = P2.Convert();
		XMVECTOR _P3 = P3.Convert();

		float Dist = 0.f;

		bool Intersect = DirectX::TriangleTests::Intersects(v1, Dir, _P1, _P2, _P3, Dist);

		if (Intersect)
		{
			return true;
		}

		// 인접 Polygon을 돌아서 어떤 polygon으로 옮겨갔는지 확인한다.
		// 모든 인접 폴리곤을 돌았는데도 교차하는 polygon이 없다면 이동 불가능한 곳으로 이동하려는 것이다
		else
		{
			std::vector<int> vecAdjPolyIndex;
			m_NavMeshComponent->GetAdjPolyIndex(m_PlayerPolyIndex, vecAdjPolyIndex);

			size_t Count = vecAdjPolyIndex.size();

			for (size_t i = 0; i < Count; ++i)
			{
				std::vector<Vector3> vecPos;
				int AdjPolyIndex = vecAdjPolyIndex[i];
				m_NavMeshComponent->GetNavPolgonVertexPos(AdjPolyIndex, vecPos);

				P1 = vecPos[0];
				P2 = vecPos[1];
				P3 = vecPos[2];

				P1 = P1.TransformCoord(WorldMat);
				P2 = P2.TransformCoord(WorldMat);
				P3 = P3.TransformCoord(WorldMat);

				XMVECTOR _P1 = P1.Convert();
				XMVECTOR _P2 = P2.Convert();
				XMVECTOR _P3 = P3.Convert();

				float Dist = 0.f;

				bool Intersect = DirectX::TriangleTests::Intersects(v1, Dir, _P1, _P2, _P3, Dist);

				if (Intersect)
				{
					m_PlayerPolyIndex = AdjPolyIndex;
					return true;
				}
			}

			return false;
		}
	}

	return false;
}

void CNavigation3DManager::Start()
{
}

bool CNavigation3DManager::Init()
{
	return true;
}

void CNavigation3DManager::Update(float DeltaTime)
{
}
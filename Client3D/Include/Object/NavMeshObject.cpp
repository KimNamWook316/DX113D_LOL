
#include "NavMeshObject.h"
#include "Scene/Scene.h"
#include "Resource/Mesh/NavMesh.h"

CNavMeshObject::CNavMeshObject()
{
	SetTypeID<CNavMeshObject>();
}

CNavMeshObject::CNavMeshObject(const CNavMeshObject& obj)	:
	CGameObject(obj)
{
}

CNavMeshObject::~CNavMeshObject()
{
}

bool CNavMeshObject::Init()
{
	m_NavMeshComponent = CreateComponent<CNavMeshComponent>("NavMeshComponent");
	m_Scene->GetNavigation3DManager()->SetNavMeshData(m_NavMeshComponent);

	SetWorldScale(2.f, 1.f, 2.f);

	// 
	m_NavMeshComponent->GetTransform()->ForceUpdateMat();

	// 먼저 플레이어가 초기에 위치할 polygon의 index를 찾아주기 위해 최초로 한번 CheckPlayerNavMeshPoly를 호출해주고
	// 매 프레임마다 CheckPlayerNavMeshPoly를 호출해서 플레이어가 이동가능한 polygon에 이동하려는지 체크하는데 그때 이미 NavMesh의 transform 행렬 정보가
	// 완성되어 있어야 한다. 그런데 transform::PostUpdate에서 행렬 정보를 완성하는데 player보다 먼저 NavMeshComponent가 PostUpdate된다는 보장이 없으므로
	// 위에서 강제로 transform정보를 완성해주는 ForceUpdateMat를 호출해준다
	m_Scene->GetNavigation3DManager()->CheckPlayerNavMeshPoly();

	//m_NavMeshComponent->CreateNavMesh("NavMesh");
	//CNavMesh* Mesh = (CNavMesh*)m_Scene->GetResource()->FindMesh("NavMesh");

	//m_NavMeshComponent->CopyNavMeshPolygonInfo(Mesh->GetNavMeshPolygonVec());
	//m_NavMeshComponent->CheckAdjInfo();

	return true;
}

void CNavMeshObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CNavMeshObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CNavMeshObject* CNavMeshObject::Clone()
{
	return new CNavMeshObject(*this);
}


#include "NavMeshComponent.h"
#include "../Resource/Mesh/NavMesh.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CNavMeshComponent::CNavMeshComponent()	:
	m_DebugRender(false),
	m_PlayerSpawnPolyIndex(0)
{
	SetTypeID<CNavMeshComponent>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_LayerName = "Default";
}

CNavMeshComponent::CNavMeshComponent(const CNavMeshComponent& com)	:
	CSceneComponent(com)
{
}

CNavMeshComponent::~CNavMeshComponent()
{

}

void CNavMeshComponent::SetNavMesh(const std::string& Name)
{
	m_NavMesh = (CNavMesh*)CResourceManager::GetInst()->FindMesh("NavMesh");

	m_Scene->GetNavigation3DManager()->SetNavMeshData(this);
}

void CNavMeshComponent::SetNavMesh(CNavMesh* NavMesh)
{
	m_NavMesh = NavMesh;
}

void CNavMeshComponent::Start()
{
	CSceneComponent::Start();
}

bool CNavMeshComponent::Init()
{
	CSceneComponent::Init();

	m_Shader = m_Scene->GetResource()->FindShader("NavMeshShader");

	SetNavMesh("NavMesh");

	SetDebugRender(true);

    return true;
}

void CNavMeshComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CNavMeshComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CNavMeshComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CNavMeshComponent::Render()
{
#ifdef _DEBUG
	
	if (m_DebugRender)
	{
		CSceneComponent::Render();

		m_WireFrameState->SetState();

		if (m_Shader)
			m_Shader->SetShader();

		if (m_NavMesh)
			m_NavMesh->Render();

		m_WireFrameState->ResetState();
	}
#endif // _DEBUG
}

void CNavMeshComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CNavMeshComponent* CNavMeshComponent::Clone()
{
	return new CNavMeshComponent(*this);
}

void CNavMeshComponent::GetNavPolgonVertexPos(int Index, std::vector<Vector3>& vecPos)
{
	NavMeshPolygon Polygon = m_NavMesh->GetNavMeshPolygon(Index);

	size_t Count = Polygon.m_vecVertexPos.size();

	for (size_t i = 0; i < Count; ++i)
		vecPos.push_back(Polygon.m_vecVertexPos[i]);
}

void CNavMeshComponent::GetAdjPolyIndex(int Index, std::vector<int>& vecPolyIndex)
{
	NavMeshPolygon Polygon = m_NavMesh->GetNavMeshPolygon(Index);

	size_t Count = Polygon.m_vecAdjIndex.size();

	for (size_t i = 0; i < Count; ++i)
		vecPolyIndex.push_back(Polygon.m_vecAdjIndex[i]);
}

const Vector3& CNavMeshComponent::GetVertexPos(int PolyIndex, int VertexIndex)
{
	return m_NavMesh->GetNavMeshPolygon(PolyIndex).m_vecVertexPos[VertexIndex];
}


bool CNavMeshComponent::Save(FILE* File)
{
	CSceneComponent::Save(File);

	m_NavMesh->SaveMesh(File);

	return true;
}

bool CNavMeshComponent::Load(FILE* File)
{
	CSceneComponent::Load(File);

	m_NavMesh = new CNavMesh;

	m_NavMesh->LoadMesh(File);

	m_Scene->GetNavigation3DManager()->SetNavMeshData(this);

	return true;
}

bool CNavMeshComponent::SaveOnly(FILE* File)
{
	return false;
}

bool CNavMeshComponent::LoadOnly(FILE* File)
{
	return false;
}
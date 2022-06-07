
#include "SkyObject.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Material/Material.h"

CSkyObject::CSkyObject()
{
	SetTypeID<CSkyObject>();
}

CSkyObject::CSkyObject(const CSkyObject& obj) :
	CGameObject(obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CSkyObject::~CSkyObject()
{
}

bool CSkyObject::Init()
{
	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	m_Mesh->SetMesh("SpherePos");

	CMaterial* Material = m_Scene->GetResource()->FindMaterial("SkyMaterial");

	// FrontFace Culling 으로 안에서 밖으로 바라보는 시점에서도 Sky Mesh 출력을 수행하도록 한다.
	// SkyDepth State에서는 최종 투영 변환된 z값의 출력 상한을 1과 같거나 작은 경우로 설정한다.
	// Shader에서 이 메쉬의 z값을 무조건 1로 변경해줌.
	Material->SetRenderState("FrontFaceCull");
	Material->SetRenderState("SkyDepth");

	m_Mesh->AddMaterial(Material);

	m_Mesh->SetLayerName("Back");

	m_Mesh->SetRelativeScale(100000.f, 100000.f, 100000.f);

	return true;
}

void CSkyObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CSkyObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CSkyObject* CSkyObject::Clone()
{
	return new CSkyObject(*this);
}

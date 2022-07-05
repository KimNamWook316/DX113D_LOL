#include "MapObj.h"

CMapObj::CMapObj()
{
	SetTypeID<CMapObj>();
}

CMapObj::CMapObj(const CMapObj& obj) :
	CGameObject(obj)
{
	m_Mesh = (CStaticMeshComponent*)FindComponent("Mesh");
}

CMapObj::~CMapObj()
{
}

bool CMapObj::Init()
{
	m_Mesh = CreateComponent<CStaticMeshComponent>("Mesh");

	m_Mesh->SetMesh("MapMesh");

	SetRelativeScale(5.0f, 5.0f, 5.0f);

	AddWorldPos(0.f, -5.f,0.f);

	// AddWorldRotationX(-90.f);

	return true;
}

void CMapObj::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CMapObj::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CMapObj* CMapObj::Clone()
{
	return new CMapObj(*this);
}

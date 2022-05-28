
#include "LandScapeObj.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "d3dx10math.h"

CLandScapeObj::CLandScapeObj()
{
	SetTypeID<CLandScapeObj>();
}

CLandScapeObj::CLandScapeObj(const CLandScapeObj& obj) :
	CGameObject(obj)
{
	m_LandScape = (CLandScape*)FindComponent("LandScape");
}

CLandScapeObj::~CLandScapeObj()
{
}

bool CLandScapeObj::Init()
{
	m_LandScape = CreateComponent<CLandScape>("LandScape");

	m_LandScape->CreateLandScape("LandScape", 129, 129,
		TEXT("LandScape/height1.bmp"));

	m_LandScape->AddMaterial("LandScape");

	m_LandScape->SetDetailLevel(30.f);
	m_LandScape->SetSplatCount(4);


	return true;
}

void CLandScapeObj::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CLandScapeObj::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	Vector3 LeftBottom = m_LandScape->GetMin();
	Vector3 RightTop = m_LandScape->GetMax();

	float LeftTopHeight = m_LandScape->GetHeight(Vector3(LeftBottom.x, 0.f, RightTop.z));
	Vector3 LeftTop = Vector3(LeftBottom.x, LeftTopHeight, RightTop.z);

	float RightBottomHeight = m_LandScape->GetHeight(Vector3(RightTop.x, 0.f, LeftBottom.z));
	Vector3 RightBottom = Vector3(RightTop.x, RightBottomHeight, LeftBottom.z);

	Matrix matWorld = m_LandScape->GetTransform()->GetWorldMatrix();

	CCameraComponent* CurrentCamera = m_Scene->GetCameraManager()->GetCurrentCamera();
	Matrix matView = CurrentCamera->GetViewMatrix();
	Ray ray = CInput::GetInst()->GetRay(matWorld);

	LeftBottom = LeftBottom.TransformCoord(matWorld);
	LeftBottom = LeftBottom.TransformCoord(matView);
	RightBottom = RightBottom.TransformCoord(matWorld);
	RightBottom = RightBottom.TransformCoord(matView);
	LeftTop = LeftBottom.TransformCoord(matWorld);
	LeftTop = LeftBottom.TransformCoord(matView);

	float* pU = nullptr;
	float* pV = nullptr;
	float* pDist = nullptr;
	
	const D3DXVECTOR3 p0 = { LeftBottom.x, LeftBottom.y, LeftBottom.z };
	const D3DXVECTOR3 p1 = { LeftBottom.x, LeftBottom.y, LeftBottom.z };
	const D3DXVECTOR3 p2 = { LeftBottom.x, LeftBottom.y, LeftBottom.z };

	const D3DXVECTOR3 rayPos = { ray.Pos.x, ray.Pos.y, ray.Pos.z };
	
	const D3DXVECTOR3 rayDir = { ray.Dir.x, ray.Dir.y, ray.Dir.z };

	//bool Intersect = D3DXIntersectTri(&p0, &p1, &p2, &rayPos, &rayDir, pU, pV, pDist);

	//if (Intersect)
	//{
	//	int a = 3;
	//}
}

CLandScapeObj* CLandScapeObj::Clone()
{
	return new CLandScapeObj(*this);
}


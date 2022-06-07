
#include "ColliderBox3D.h"
#include "ColliderBox2D.h"
#include "ColliderCircle.h"
#include "ColliderPixel.h"
#include "../Collision/Collision.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"

CColliderBox3D::CColliderBox3D()
{
	SetTypeID<CColliderBox3D>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_ColliderType = Collider_Type::Box3D;
}

CColliderBox3D::CColliderBox3D(const CColliderBox3D& com) :
	CColliderComponent(com)
{
	m_Info = com.m_Info;
}

CColliderBox3D::~CColliderBox3D()
{
}

void CColliderBox3D::Start()
{
	CColliderComponent::Start();
}

bool CColliderBox3D::Init()
{
	if (!CColliderComponent::Init())
		return false;

	m_Info.Axis[0] = Vector3(1.f, 0.f, 0.f);
	m_Info.Axis[1] = Vector3(0.f, 1.f, 0.f);
	m_Info.Axis[2] = Vector3(0.f, 0.f, 1.f);

	m_Info.AxisLen[0] = 0.5f;
	m_Info.AxisLen[1] = 0.5f;
	m_Info.AxisLen[2] = 0.5f;

	m_Info.Min.x = m_Info.Center.x - m_Info.Axis[0].x * m_Info.AxisLen[0];
	m_Info.Min.y = m_Info.Center.y - m_Info.Axis[1].y * m_Info.AxisLen[1];
	m_Info.Min.z = m_Info.Center.z - m_Info.Axis[2].z * m_Info.AxisLen[2];

	m_Info.Max.x = m_Info.Center.x + m_Info.Axis[0].x * m_Info.AxisLen[0];
	m_Info.Max.y = m_Info.Center.y + m_Info.Axis[1].y * m_Info.AxisLen[1];
	m_Info.Max.z = m_Info.Center.z + m_Info.Axis[2].z * m_Info.AxisLen[2];

	SetInheritRotZ(true);

	SetWorldScale(m_Info.AxisLen[0] * 2.f, m_Info.AxisLen[1] * 2.f, m_Info.AxisLen[2] * 2.f);

	m_Mesh = m_Scene->GetResource()->FindMesh("CubeLinePos");

	return true;
}

void CColliderBox3D::Update(float DeltaTime)
{
	CColliderComponent::Update(DeltaTime);
}

void CColliderBox3D::PostUpdate(float DeltaTime)
{
	CColliderComponent::PostUpdate(DeltaTime);

	m_Info.Center.x = GetWorldPos().x + m_Offset.x;
	m_Info.Center.y = GetWorldPos().y + m_Offset.y;
	m_Info.Center.z = GetWorldPos().z + m_Offset.z;

	m_Info.Axis[0].x = GetWorldAxis(AXIS_X).x;
	m_Info.Axis[0].y = GetWorldAxis(AXIS_X).y;
	m_Info.Axis[0].z = GetWorldAxis(AXIS_X).z;

	m_Info.Axis[1].x = GetWorldAxis(AXIS_Y).x;
	m_Info.Axis[1].y = GetWorldAxis(AXIS_Y).y;
	m_Info.Axis[1].z = GetWorldAxis(AXIS_Y).z;

	m_Info.Axis[2].x = GetWorldAxis(AXIS_Z).x;
	m_Info.Axis[2].y = GetWorldAxis(AXIS_Z).y;
	m_Info.Axis[2].z = GetWorldAxis(AXIS_Z).z;

	m_Info.AxisLen[0] = GetWorldScale().x / 2.f;
	m_Info.AxisLen[1] = GetWorldScale().y / 2.f;
	m_Info.AxisLen[2] = GetWorldScale().z / 2.f;

	m_Info.Min.x = m_Info.Center.x - m_Info.AxisLen[0];
	m_Info.Min.y = m_Info.Center.y - m_Info.AxisLen[1];
	m_Info.Min.z = m_Info.Center.z - m_Info.AxisLen[2];

	m_Info.Max.x = m_Info.Center.x + m_Info.AxisLen[0];
	m_Info.Max.y = m_Info.Center.y + m_Info.AxisLen[1];
	m_Info.Max.z = m_Info.Center.z + m_Info.AxisLen[2];


	//m_Info.Min.x = m_Info.Center.x - m_Info.Axis[0].x * m_Info.AxisLen[0];
	//m_Info.Min.y = m_Info.Center.y - m_Info.Axis[1].y * m_Info.AxisLen[1];
	//m_Info.Min.z = m_Info.Center.z - m_Info.Axis[2].z * m_Info.AxisLen[2];

	//m_Info.Max.x = m_Info.Center.x + m_Info.Axis[0].x * m_Info.AxisLen[0];
	//m_Info.Max.y = m_Info.Center.y + m_Info.Axis[1].y * m_Info.AxisLen[1];
	//m_Info.Max.z = m_Info.Center.z + m_Info.Axis[2].z * m_Info.AxisLen[2];

	//float MinX = min(m_Info.Min.x, m_Info.Max.x);
	//float MinY = min(m_Info.Min.y, m_Info.Max.y);
	//float MinZ = min(m_Info.Min.z, m_Info.Max.z);

	//float MaxX = max(m_Info.Min.x, m_Info.Max.x);
	//float MaxY = max(m_Info.Min.y, m_Info.Max.y);
	//float MaxZ = max(m_Info.Min.z, m_Info.Max.z);

	//m_Info.Min = Vector3(MinX, MinY, MinZ);
	//m_Info.Max = Vector3(MaxX, MaxY, MaxZ);

}

void CColliderBox3D::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderBox3D::Render()
{
	CColliderComponent::Render();

	CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	Matrix	matWorld, matView, matProj, matWVP;

	matView = Camera->GetViewMatrix();
	matProj = Camera->GetProjMatrix();

	Matrix	matScale, matRot, matTrans;

	matScale.Scaling(m_Info.AxisLen[0] * 2.f, m_Info.AxisLen[1] * 2.f, m_Info.AxisLen[2] * 2.f);
	matRot.Rotation(GetWorldRot());
	matTrans.Translation(m_Info.Center);

	matWorld = matScale * matRot * matTrans;

	matWVP = matWorld * matView * matProj;

	matWVP.Transpose();

	m_CBuffer->SetWVP(matWVP);

	if (m_PrevCollisionList.empty())
		m_CBuffer->SetColliderColor(Vector4(0.f, 1.f, 0.f, 1.f));

	else
		m_CBuffer->SetColliderColor(Vector4(1.f, 0.f, 0.f, 1.f));

	if (m_MouseCollision)
		m_CBuffer->SetColliderColor(Vector4(1.f, 0.f, 0.f, 1.f));

	m_CBuffer->UpdateCBuffer();

	m_Shader->SetShader();

	m_Mesh->Render();
}

void CColliderBox3D::PostRender()
{
	CColliderComponent::PostRender();
}

CColliderBox3D* CColliderBox3D::Clone()
{
	return new CColliderBox3D(*this);
}

bool CColliderBox3D::Save(FILE* File)
{

	return true;
}

bool CColliderBox3D::Load(FILE* File)
{
	return true;
}

bool CColliderBox3D::Collision(CColliderComponent* Dest)
{
	switch (Dest->GetColliderType())
	{
		//case Collider_Type::Box2D:
		//	return CCollision::CollisionBox2DToBox2D(this, (CColliderBox2D*)Dest);
		//case Collider_Type::Circle:
		//	return CCollision::CollisionBox2DToCircle(this, (CColliderCircle*)Dest);
		//case Collider_Type::Pixel:
		//	return CCollision::CollisionBox2DToPixel(this, (CColliderPixel*)Dest);
	case Collider_Type::Box3D:
		return CCollision::CollisionBox3DToBox3D(this, (CColliderBox3D*)Dest);
	}
}

bool CColliderBox3D::CollisionMouse(const Vector2& MousePos)
{
	return false;
}

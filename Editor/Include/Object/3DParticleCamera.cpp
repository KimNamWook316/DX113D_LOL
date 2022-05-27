#include "3DParticleCamera.h"
#include "Component/CameraComponent.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"

C3DParticleCamera::C3DParticleCamera() :
	m_Rotate(false),
	m_Zoom(false)
{
}

C3DParticleCamera::C3DParticleCamera(const C3DParticleCamera& com) :
	CGameObject(com)
{
}

C3DParticleCamera::~C3DParticleCamera()
{
}

void C3DParticleCamera::SetCameraSeeFromUpToDown()
{
}

void C3DParticleCamera::SetCameraSeeFront()
{
}

void C3DParticleCamera::SetCameraSeeDiagonal()
{
}

void C3DParticleCamera::Start()
{
}

bool C3DParticleCamera::Init()
{
	m_Camera = CreateComponent<CCameraComponent>("Camera");

	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);
	m_Camera->SetInheritRotZ(true);
	m_Camera->SetInheritParentRotationPosX(true);
	m_Camera->SetInheritParentRotationPosY(true);
	m_Camera->SetInheritParentRotationPosZ(true);

	m_Camera->SetCameraType(Camera_Type::Camera3D);

	// m_Scene->GetCameraManager()->SetParticleEditorCamera(m_Camera);

	return true;
}

void C3DParticleCamera::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	// Target 과의 거리 세팅
	if (m_TargetObject)
	{
		Vector3	TargetPos = m_TargetObject->GetWorldPos();

		Vector3	Pos = TargetPos - GetWorldAxis(AXIS_Z) * m_TargetDistance;

		SetWorldPos(Pos + m_Offset);
	}

	// Zoom In 기능
	if (CInput::GetInst()->GetWheelDir() && m_Zoom)
	{
		m_TargetDistance += CInput::GetInst()->GetWheelDir() * 0.5f;
	}

	if (m_Rotate)
	{
		AddRelativeRotationY(60.f * DeltaTime);
	}
}


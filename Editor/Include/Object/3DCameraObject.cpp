
#include "3DCameraObject.h"
#include "Component/CameraComponent.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/CameraManager.h"

#define EDITOR_CAM_SPEED 4.0f

C3DCameraObject::C3DCameraObject()
{
	SetTypeID<C3DCameraObject>();
}

C3DCameraObject::C3DCameraObject(const C3DCameraObject& obj)	:
	CGameObject(obj)
{
}

C3DCameraObject::~C3DCameraObject()
{
}

bool C3DCameraObject::Init()
{
	CGameObject::Init();

	m_FocusPoint = CreateComponent<CSceneComponent>("Focus");
	m_Camera = CreateComponent<CCameraComponent>("Camera");

	m_FocusPoint->AddChild(m_Camera);
	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);
	m_Camera->SetInheritRotZ(true);
	m_Camera->SetInheritParentRotationPosX(true);
	m_Camera->SetInheritParentRotationPosY(true);
	m_Camera->SetInheritParentRotationPosZ(true);

	m_Camera->SetCameraType(Camera_Type::Camera3D);
	m_Camera->SetViewAngle(27.f);

	m_Camera->AddRelativePos(0.f, 30.f, -30.f);
	m_Camera->SetRelativeRotationX(45.f);

	// Input Key 설정
	CInput::GetInst()->CreateKey("CameraMove", VK_LBUTTON);
	CInput::GetInst()->CreateKey("CameraRot", VK_RBUTTON);

	// Ctrl + Mouse L Drag -> 시점 이동 
	CInput::GetInst()->SetCtrlKey("CameraMove", true);
	CInput::GetInst()->SetKeyCallback("CameraMove", KeyState_Push, this, &C3DCameraObject::OnDragMove);

	// Ctrl + Mouse R  Drag -> 시점 회전
	CInput::GetInst()->SetCtrlKey("CameraRot", true);
	CInput::GetInst()->SetKeyCallback("CameraRot", KeyState_Push, this, &C3DCameraObject::OnDragRotate);

	// Main Camera로 설정
	m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);

	return true;
}

void C3DCameraObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (CInput::GetInst()->GetWheelDir())
	{
		Vector3 AxisZ = m_Camera->GetWorldAxis(AXIS::AXIS_Z);
		AxisZ *= CInput::GetInst()->GetWheelDir() * -EDITOR_CAM_SPEED;
		m_Camera->AddWorldPos(AxisZ);
	}
}

void C3DCameraObject::OnDragMove(float DeltaTime)
{
	Vector3 moveAmount = Vector3(-1.f * CInput::GetInst()->GetMouseMove().x, CInput::GetInst()->GetMouseMove().y, 0.f);

	moveAmount *= EDITOR_CAM_SPEED;

	m_Camera->AddRelativePos(moveAmount);
}

void C3DCameraObject::OnDragRotate(float DeltaTime)
{
	Vector3 moveAmount = Vector3(-1.f * CInput::GetInst()->GetMouseMove().x, CInput::GetInst()->GetMouseMove().y, 0.f);
	
	Vector3 Rotation = Vector3(moveAmount.y, moveAmount.x, 0.f);

	Rotation  *= EDITOR_CAM_SPEED;

	AddWorldRotation(Rotation);
}

void C3DCameraObject::OnZoomIn(float DetlaTime)
{
}

void C3DCameraObject::OnZoomOut(float DetlaTime)
{
}

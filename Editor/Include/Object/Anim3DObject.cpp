#include "Anim3DObject.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/Navigation3DManager.h"
#include "../EditorManager.h"
#include "../Window/AnimationEditor.h"

CAnim3DObject::CAnim3DObject()
{
	SetTypeID<CAnim3DObject>();
}

CAnim3DObject::CAnim3DObject(const CAnim3DObject& obj) :
	CGameObject(obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Arm = (CArm*)FindComponent("Arm");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
}

CAnim3DObject::~CAnim3DObject()
{
}

bool CAnim3DObject::Init()
{
	m_Mesh = CreateComponent<CAnimationMeshComponent>("Mesh");
	m_Mesh->SetLayerName("AnimationEditorLayer");
	
	m_Arm = CreateComponent<CArm>("Arm");
	m_Camera = CreateComponent<CCameraComponent>("Camera");

	m_Mesh->AddChild(m_Arm);
	m_Arm->AddChild(m_Camera);

	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);
	m_Camera->SetInheritRotZ(true);

	// m_Mesh->SetMesh("PlayerMesh");
	m_Mesh->SetMesh(CEditorManager::GetInst()->GetAnimationEditor()->Get3DTestObjectMeshName());

	// GBuffer 가 아니라, 바로 Animation Editor 용 Render Target 에 그려내기 위해 Shader 를 다른 것으로 세팅한다.
	// --> 그런데 이 코드가 있으면 오류가 난다.
	m_Mesh->SetMaterialShader("Mesh3DNoLightShader");

	// m_Mesh->CreateAnimationInstance<CAnim3DObjectAnimation>();
	// m_Animation = (CAnim3DObjectAnimation*)m_Mesh->GetAnimationInstance();

	m_Mesh->SetRelativeScale(0.02f, 0.02f, 0.02f);

	m_Arm->SetOffset(0.f, 2.f, 0.f);
	m_Arm->SetRelativeRotation(10.f, 0.f, 0.f);
	m_Arm->SetTargetDistance(10.f);

	SetWorldPos(1.f, 1.f, 1.f);

	// Animation Editor Camera 에 적용받는 대상으로 세팅하기
	// m_RootComponent->SetAnimationEditorTargetEnable(true);

	// 3DTestObject 의 Camera Object 를 Scene의 Animation Current Camera 로 세팅한다.
	m_Scene->GetCameraManager()->SetAnimationEditorCamera(m_Camera);
	// m_Scene->GetCameraManager()->SetCurrentCamera(m_Camera);

	return true;
}

void CAnim3DObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (CInput::GetInst()->GetWheelDir())
	{
		float Length = m_Arm->GetTargetDistance() +
			CInput::GetInst()->GetWheelDir() * 0.3f;

		m_Arm->SetTargetDistance(Length);
	}

	if (m_IsCameraRot)
	{
		m_Arm->AddRelativeRotationY(45.f * DeltaTime);
	}

	/*
	if (m_Velocity.Length() > 0.f)
	{
		//m_Animation->ChangeAnimation("Walk");
		m_Animation->SetIdleEnable(true);
	}

	else if (m_Animation->GetIdleEnable())
	{
		m_Animation->ChangeAnimation("Idle");
	}
	*/
}

void CAnim3DObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	//Vector3	Pos = GetWorldPos();

	//Pos.y = m_Scene->GetNavigation3DManager()->GetY(Pos);

	//SetWorldPos(Pos);

	m_Velocity = Vector3::Zero;
}

CAnim3DObject* CAnim3DObject::Clone()
{
	return new CAnim3DObject(*this);
}

void CAnim3DObject::MoveFront(float DeltaTime)
{
	m_Velocity += GetWorldAxis(AXIS_Z) * 10.f * DeltaTime;

	AddWorldPos(GetWorldAxis(AXIS_Z) * 10.f * DeltaTime);
}

void CAnim3DObject::MoveBack(float DeltaTime)
{
	m_Velocity += GetWorldAxis(AXIS_Z) * -10.f * DeltaTime;

	AddWorldPos(GetWorldAxis(AXIS_Z) * -10.f * DeltaTime);
}

void CAnim3DObject::RotationYInv(float DeltaTime)
{
	AddWorldRotationY(-180.f * DeltaTime);
}

void CAnim3DObject::RotationY(float DeltaTime)
{
	AddWorldRotationY(180.f * DeltaTime);
}

void CAnim3DObject::Attack(float DeltaTime)
{
	/*
	if (m_Animation)
	{
		m_Animation->ChangeAnimation("Attack");
		m_Animation->SetIdleEnable(false);
	}
	*/
}

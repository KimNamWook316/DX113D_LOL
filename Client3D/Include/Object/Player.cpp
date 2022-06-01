
#include "Player.h"
#include "PlayerAnimation.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/Navigation3DManager.h"
#include "Weapon.h"
#include "Component/StateComponent.h"
#include "Component/ColliderBox3D.h"
#include "Collision/Collision.h"

CPlayer::CPlayer()	:
	m_FrameCount(0)
{
	SetTypeID<CPlayer>();
}

CPlayer::CPlayer(const CPlayer& obj) :
	CGameObject(obj)
{
	m_Mesh = (CAnimationMeshComponent*)FindComponent("Mesh");
	m_Arm = (CArm*)FindComponent("Arm");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	m_Mesh = CreateComponent<CAnimationMeshComponent>("PlayerMesh");
	m_Arm = CreateComponent<CArm>("Arm");
	m_Camera = CreateComponent<CCameraComponent>("Camera");
	m_Body = CreateComponent<CColliderBox3D>("Body");

	m_Mesh->AddChild(m_Arm);


	m_Arm->AddChild(m_Camera);

	m_Camera->SetInheritRotX(true);
	m_Camera->SetInheritRotY(true);
	m_Camera->SetInheritRotZ(true);

	m_Mesh->SetMesh("PlayerMesh");
	m_Mesh->SetReceiveDecal(false);



	m_Mesh->CreateAnimationInstance<CPlayerAnimation>();

	m_Animation = (CPlayerAnimation*)m_Mesh->GetAnimationInstance();

	m_Mesh->SetRelativeScale(0.02f, 0.02f, 0.02f);

	m_Arm->SetOffset(0.f, 2.f, 0.f);
	m_Arm->SetRelativeRotation(40.f, 0.f, 0.f);
	m_Arm->SetTargetDistance(10.f);

	m_Weapon = m_Scene->CreateGameObject<CWeapon>("Weapon");

	m_Mesh->AddChild(m_Weapon, "Weapon");



	// ColliderBox3D Test
	m_Mesh->AddChild(m_Body);

	m_Body->SetInheritRotX(true);
	m_Body->SetInheritRotY(true);
	m_Body->SetInheritRotZ(true);
	m_Body->SetExtent(1.f, 1.f, 1.f);
	m_Body->SetOffset(0.f, 2.f, 0.f);



	CInput::GetInst()->SetKeyCallback<CPlayer>("MoveFront", KeyState_Push,
		this, &CPlayer::MoveFront);
	CInput::GetInst()->SetKeyCallback<CPlayer>("MoveBack", KeyState_Push,
		this, &CPlayer::MoveBack);
	CInput::GetInst()->SetKeyCallback<CPlayer>("RotationYInv", KeyState_Push,
		this, &CPlayer::RotationYInv);
	CInput::GetInst()->SetKeyCallback<CPlayer>("RotationY", KeyState_Push,
		this, &CPlayer::RotationY);

	CInput::GetInst()->SetKeyCallback<CPlayer>("Attack1", KeyState_Down,
		this, &CPlayer::Attack);


	return true;
}

void CPlayer::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	if (CInput::GetInst()->GetWheelDir())
	{
		float Length = m_Arm->GetTargetDistance() +
			CInput::GetInst()->GetWheelDir() * 0.1f;

		m_Arm->SetTargetDistance(Length);
	}

	//m_Arm->AddRelativeRotationY(90.f * DeltaTime);
	if (m_Velocity.Length() > 0.f)
	{
		//m_Animation->ChangeAnimation("Walk");
		m_Animation->SetIdleEnable(true);
	}

	else if (m_Animation->GetIdleEnable())
	{
		m_Animation->ChangeAnimation("Idle");
	}



	CGameObject* PickObj = nullptr;

	if (m_Scene->Picking(PickObj))
	{
		int a = 0;
	}
	
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

	Vector3	Pos = GetWorldPos();

	Pos.y = m_Scene->GetNavigation3DManager()->GetY(Pos);

	SetWorldPos(Pos);

	m_Velocity = Vector3::Zero;


	// Ray vs Box Test Code //

	//IsBoxRayCollision();
	
	//////////////////////////
}

CPlayer* CPlayer::Clone()
{
	return new CPlayer(*this);
}

void CPlayer::MoveFront(float DeltaTime)
{
	m_Velocity += GetWorldAxis(AXIS_Z) * 10.f * DeltaTime;

	AddWorldPos(GetWorldAxis(AXIS_Z) * 10.f * DeltaTime);
}

void CPlayer::MoveBack(float DeltaTime)
{
	m_Velocity += GetWorldAxis(AXIS_Z) * -10.f * DeltaTime;

	AddWorldPos(GetWorldAxis(AXIS_Z) * -10.f * DeltaTime);
}

void CPlayer::RotationYInv(float DeltaTime)
{
	AddWorldRotationY(-180.f * DeltaTime);
}

void CPlayer::RotationY(float DeltaTime)
{
	AddWorldRotationY(180.f * DeltaTime);
}

void CPlayer::Attack(float DeltaTime)
{
	m_Animation->ChangeAnimation("Attack");
	m_Animation->SetIdleEnable(false);
}

bool CPlayer::IsBoxRayCollision()
{
	++m_FrameCount;

	if (CInput::GetInst()->GetMouseLButtonClick() && m_FrameCount > 20)
	{
		CCameraComponent* CurrentCamera = m_Scene->GetCameraManager()->GetCurrentCamera();
		Matrix matView = CurrentCamera->GetViewMatrix();
		Ray ray = CInput::GetInst()->GetRay(matView);

		Box3DInfo Info = m_Body->GetInfo();
		Matrix WorldMat = m_Body->GetWorldMatrix();

		Info.Center = Info.Center.TransformCoord(WorldMat);
		Info.Axis[0] = Info.Axis[0].TransformCoord(WorldMat);
		Info.Axis[1] = Info.Axis[1].TransformCoord(WorldMat);
		Info.Axis[2] = Info.Axis[2].TransformCoord(WorldMat);
		Info.Min = Info.Min.TransformCoord(WorldMat);
		Info.Max = Info.Max.TransformCoord(WorldMat);

		Vector3 HitPoint;

		CCollision::CollisionRayToBox3D(HitPoint, ray, Info);

		m_FrameCount = 0;
	}

	return true;
}



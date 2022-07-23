
#include "PlayerDataComponent.h"
#include "Input.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/AnimationMeshComponent.h"
#include "../DataManager.h"
#include "GameStateComponent.h"
#include "Component/ColliderComponent.h"
#include "Scene/Navigation3DManager.h"

CPlayerDataComponent::CPlayerDataComponent()	:
	m_OnSlash(false),
	m_AnimComp(nullptr),
	m_Body(nullptr),
	m_FrameCount(0),
	m_MouseLButtonDown(false)

{
	SetTypeID<CPlayerDataComponent>();
	m_ComponentType = Component_Type::ObjectComponent;
}

CPlayerDataComponent::CPlayerDataComponent(const CPlayerDataComponent& com)	:
	CObjectDataComponent(com)
{
}

CPlayerDataComponent::~CPlayerDataComponent()
{
}

void CPlayerDataComponent::Start()
{
	m_Scene->GetSceneMode()->SetPlayerObject(m_Object);

	CInput::GetInst()->CreateKey("WeaponArrow", '1');
	CInput::GetInst()->CreateKey("WeaponFire", '2');
	CInput::GetInst()->CreateKey("WeaponChain", '3');
	CInput::GetInst()->CreateKey("MoveForward", 'W');
	CInput::GetInst()->CreateKey("MoveBack", 'S');
	CInput::GetInst()->CreateKey("MoveLeft", 'A');
	CInput::GetInst()->CreateKey("MoveRight", 'D');
	CInput::GetInst()->CreateKey("Roll", VK_SPACE);

	CInput::GetInst()->SetKeyCallback("WeaponArrow", KeyState_Down, this, &CPlayerDataComponent::SetPlayerAbilityArrow);
	CInput::GetInst()->SetKeyCallback("WeaponFire", KeyState_Down, this, &CPlayerDataComponent::SetPlayerAbilityFire);
	CInput::GetInst()->SetKeyCallback("WeaponChain", KeyState_Down, this, &CPlayerDataComponent::SetPlayerAbilityChain);

	m_AnimComp = m_Object->FindComponentFromType<CAnimationMeshComponent>();

	// Player Animation Notify는 여기 추가
	m_AnimComp->GetAnimationInstance()->AddNotify<CPlayerDataComponent>("PlayerSlashL", "PlayerSlashL", 3, this, &CPlayerDataComponent::SetTrueOnSlash);
	m_AnimComp->GetAnimationInstance()->AddNotify<CPlayerDataComponent>("PlayerSlashL", "PlayerSlashL", 8, this, &CPlayerDataComponent::SetFalseOnSlash);
	m_AnimComp->GetAnimationInstance()->AddNotify<CPlayerDataComponent>("PlayerHitBack", "PlayerHitBack", 0, this, &CPlayerDataComponent::OnHitBack);
	m_AnimComp->GetAnimationInstance()->AddNotify<CPlayerDataComponent>("PlayerRoll", "PlayerRoll", 0, this, &CPlayerDataComponent::OnRoll);
	
	m_AnimComp->GetAnimationInstance()->SetEndFunction<CPlayerDataComponent>("PlayerHitBack", this, &CPlayerDataComponent::OnHitBackEnd);
	m_AnimComp->GetAnimationInstance()->SetEndFunction<CPlayerDataComponent>("PlayerHitRecover", this, &CPlayerDataComponent::OnHitRecoverEnd);
	m_AnimComp->GetAnimationInstance()->SetEndFunction<CPlayerDataComponent>("PlayerRoll", this, &CPlayerDataComponent::OnRollEnd);

	m_Data = CDataManager::GetInst()->GetObjectData("Player");
	m_Body = (CColliderComponent*)m_Object->FindComponent("Body");

}

bool CPlayerDataComponent::Init()
{

	return true;
}

void CPlayerDataComponent::Update(float DeltaTime)
{
	//CObjectComponent::Update(DeltaTime);

	//if (m_AttackCount > 3)
	//	m_AttackCount = 0;

	m_FrameCount = 0;

	CGameStateComponent* Comp = m_Object->FindObjectComponentFromType<CGameStateComponent>();

	if (Comp->IsTreeUpdate())
	{
		//++m_FrameCount;
		bool LButtonDown = CInput::GetInst()->GetMouseLButtonClick();
		bool LButtonUp = CInput::GetInst()->GetMouseLButtonUp();

		// 
		if (LButtonDown && !m_MouseLButtonDown && !LButtonUp)
		{
			m_MouseLButtonDown = true;

			m_KeyStateQueue.push(VK_LBUTTON);

			if (m_KeyStateQueue.size() > 1)
				int a = 3;

		}

		if (LButtonUp)
			m_MouseLButtonDown = false;

	}
}

void CPlayerDataComponent::PostUpdate(float DeltaTime)
{
}

void CPlayerDataComponent::PrevRender()
{
}

void CPlayerDataComponent::Render()
{
}

void CPlayerDataComponent::PostRender()
{
}

CPlayerDataComponent* CPlayerDataComponent::Clone()
{
	return new CPlayerDataComponent(*this);
}

bool CPlayerDataComponent::Save(FILE* File)
{
	CObjectDataComponent::Save(File);

	fwrite(&m_PlayerData, sizeof(PlayerData), 1, File);

	return true;
}

bool CPlayerDataComponent::Load(FILE* File)
{
	CObjectDataComponent::Load(File);

	fread(&m_PlayerData, sizeof(PlayerData), 1, File);

	return true;
}

bool CPlayerDataComponent::SaveOnly(FILE* File)
{
	CObjectDataComponent::Save(File);

	fwrite(&m_PlayerData, sizeof(PlayerData), 1, File);

	return true;
}

bool CPlayerDataComponent::LoadOnly(FILE* File)
{
	CObjectDataComponent::Load(File);

	fread(&m_PlayerData, sizeof(PlayerData), 1, File);

	return true;
}

CAnimationMeshComponent* CPlayerDataComponent::GetAnimationMeshComponent() const
{
	return m_AnimComp;
}

void CPlayerDataComponent::OnHitBack()
{
	m_Body->Enable(false);
	m_NoInterrupt = true;
}

void CPlayerDataComponent::OnHitBackEnd()
{
	m_AnimComp->GetAnimationInstance()->ChangeAnimation("PlayerHitRecover");
}

void CPlayerDataComponent::OnHitRecoverEnd()
{
	m_Body->Enable(true);

	m_NoInterrupt = false;
	m_IsHit = false;
}

void CPlayerDataComponent::OnRoll()
{
	m_Body->Enable(false);
}

void CPlayerDataComponent::OnRollEnd()
{
	m_Body->Enable(true);
}

void CPlayerDataComponent::ForceUpdateAttackDirection()
{
	CNavigation3DManager* Manager = m_Object->GetScene()->GetNavigation3DManager();

	Vector3 TargetPos;
	Vector3 ZAxis = m_Object->GetWorldAxis(AXIS::AXIS_Z);

	Vector3 CurrentForwardVector = Vector3(-ZAxis.x, 0.f, -ZAxis.z);
	Vector3 DestForwardVector;
	Vector3 PickingPoint;
	Vector3 CrossVec;
	bool Over180;

	if (Manager->CheckNavMeshPickingPoint(PickingPoint))
	{
		Vector3 CurrentPos = m_Object->GetWorldPos();
		DestForwardVector = Vector3(PickingPoint.x, 0.f, PickingPoint.z) - Vector3(CurrentPos.x, 0.f, CurrentPos.z);
		DestForwardVector.Normalize();

		CrossVec = Vector3(CurrentForwardVector.x, CurrentForwardVector.y, CurrentForwardVector.z).Cross(Vector3(DestForwardVector.x, DestForwardVector.y, -DestForwardVector.z));

		// 반시계 방향으로 180도가 넘는다
		if (CrossVec.y < 0)
			Over180 = true;
		else
			Over180 = false;
	}

	else
	{
		PickingPoint = m_Object->GetWorldPos();
		DestForwardVector = CurrentForwardVector;
		return;
	}

	//////

	Vector3 CurrentFowardYZero = Vector3(CurrentForwardVector.x, 0.f, CurrentForwardVector.z);
	Vector3 DestForwardYZero = Vector3(DestForwardVector.x, 0.f, DestForwardVector.z);

	CrossVec = Vector3(CurrentFowardYZero.x, 0.f, CurrentFowardYZero.z).Cross(Vector3(DestForwardYZero.x, 0.f, DestForwardYZero.z));

	// 반시계 방향으로 180도가 넘는다
	if (CrossVec.y < 0)
		Over180 = true;
	else
		Over180 = false;

	float DotProduct = CurrentFowardYZero.Dot(DestForwardYZero);

	if (DotProduct > 0.99f || DotProduct < -0.99f)
	{
		return;
	}

	float Rad = acosf(DotProduct);
	float Degree = RadianToDegree(Rad);

	DestForwardVector.Normalize();

	// CurretForwardVector기준 DestForwardVector로 시계 방향으로 180도가 넘는다면
	if (Over180)
	{
		m_Object->AddWorldRotationY(-Degree);
	}

	else
	{
		m_Object->AddWorldRotationY(Degree);
	}

	m_AttackDir = DestForwardVector;
}

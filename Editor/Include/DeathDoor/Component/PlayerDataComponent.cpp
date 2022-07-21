
#include "PlayerDataComponent.h"
#include "Input.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"
#include "Component/AnimationMeshComponent.h"
#include "../DataManager.h"
#include "GameStateComponent.h"
#include "Component/ColliderComponent.h"

CPlayerDataComponent::CPlayerDataComponent()	:
	m_OnSlash(false),
	m_AnimComp(nullptr),
	m_Body(nullptr)
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
	//CGameStateComponent::Start가 PlayerDataComponent::Start보다 먼저 호출되면 GameStateComponent가 가지고 있는 m_Data는 데이터가 아무것도 들어있지 않게돼서 여기서 강제로 Set
	//m_Object->FindObjectComponentFromType<CGameStateComponent>()->GetObjectDataComponent()->SetObjectData(m_Data);

}

bool CPlayerDataComponent::Init()
{

	return true;
}

void CPlayerDataComponent::Update(float DeltaTime)
{
	//CObjectComponent::Update(DeltaTime);
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

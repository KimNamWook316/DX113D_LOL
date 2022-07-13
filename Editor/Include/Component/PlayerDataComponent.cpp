
#include "PlayerDataComponent.h"
#include "Input.h"
#include "GameObject/GameObject.h"
#include "Scene/Scene.h"

CPlayerDataComponent::CPlayerDataComponent()	:
	m_PlayerHook(nullptr)
{
	SetTypeID<CPlayerDataComponent>();
	m_ComponentType = Component_Type::ObjectComponent;
}

CPlayerDataComponent::CPlayerDataComponent(const CPlayerDataComponent& com)	:
	CObjectComponent(com)
{
}

CPlayerDataComponent::~CPlayerDataComponent()
{
}

void CPlayerDataComponent::Start()
{
	CObjectComponent::Start();

	m_Scene->GetSceneMode()->SetPlayerObject(m_Object);

	m_PlayerHook = (CPlayerHook*)m_Object->FindChildObjectByType<CPlayerHook>();

	CInput::GetInst()->CreateKey("WeaponArrow", '1');
	CInput::GetInst()->CreateKey("WeaponFire", '2');
	CInput::GetInst()->CreateKey("WeaponChain", '3');

	CInput::GetInst()->SetKeyCallback("WeaponArrow", KeyState_Down, this, &CPlayerDataComponent::SetPlayerAbilityArrow);
	CInput::GetInst()->SetKeyCallback("WeaponFire", KeyState_Down, this, &CPlayerDataComponent::SetPlayerAbilityFire);
	CInput::GetInst()->SetKeyCallback("WeaponChain", KeyState_Down, this, &CPlayerDataComponent::SetPlayerAbilityChain);
}

bool CPlayerDataComponent::Init()
{

	return true;
}

void CPlayerDataComponent::Update(float DeltaTime)
{
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
	CObjectComponent::Save(File);

	fwrite(&m_PlayerData, sizeof(PlayerData), 1, File);

	return true;
}

bool CPlayerDataComponent::Load(FILE* File)
{
	CObjectComponent::Load(File);

	fread(&m_PlayerData, sizeof(PlayerData), 1, File);

	return true;
}

bool CPlayerDataComponent::SaveOnly(FILE* File)
{
	CObjectComponent::Save(File);

	fwrite(&m_PlayerData, sizeof(PlayerData), 1, File);

	return true;
}

bool CPlayerDataComponent::LoadOnly(FILE* File)
{
	CObjectComponent::Load(File);

	fread(&m_PlayerData, sizeof(PlayerData), 1, File);

	return true;
}

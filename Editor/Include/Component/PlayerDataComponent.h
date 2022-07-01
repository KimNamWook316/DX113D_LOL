#pragma once

#include "Component/ObjectComponent.h"
#include "GameObject/GameObject.h"
#include "../Object/PlayerHook.h"

class CPlayerDataComponent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CPlayerDataComponent();
	CPlayerDataComponent(const CPlayerDataComponent& com);
	virtual ~CPlayerDataComponent();

private:
	PlayerData m_PlayerData;
	CPlayerHook* m_PlayerHook;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CPlayerDataComponent* Clone();


public:
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;

public:
	CPlayerHook* GetPlayerHookHead()	const
	{
		return m_PlayerHook;
	}

	void SetPlayerAbilityArrow(float DeltaTime)
	{
		m_PlayerData.Abilty_Type = Player_Ability::Arrow;

		//m_PlayerHook->Enable(false);

	}

	void SetPlayerAbilityFire(float DeltaTime)
	{
		m_PlayerData.Abilty_Type = Player_Ability::Fire;

		//m_PlayerHook->Enable(false);
	}

	void SetPlayerAbilityChain(float DeltaTime)
	{
		m_PlayerData.Abilty_Type = Player_Ability::Hook;
	}

	Player_Ability GetPlayerAbility()	const
	{
		return m_PlayerData.Abilty_Type;
	}
};


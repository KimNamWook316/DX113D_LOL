#pragma once

#include "ObjectDataComponent.h"
#include "GameObject/GameObject.h"

class CPlayerDataComponent :
    public CObjectDataComponent
{
	friend class CGameObject;

protected:
	CPlayerDataComponent();
	CPlayerDataComponent(const CPlayerDataComponent& com);
	virtual ~CPlayerDataComponent();

private:
	PlayerData m_PlayerData;
	bool m_OnSlash;
	class CAnimationMeshComponent* m_AnimComp;
	class CPlayerNormalAttackCheckCollider* m_AttackCheckCollider;
	class CColliderComponent* m_Body;
	Vector3 m_ShootDir;

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
	void SetTrueOnSlash();

	void SetFalseOnSlash();

	void SetOnSlash(bool Slash)
	{
		m_OnSlash = Slash;
	}

	bool GetOnSlash()	const
	{
		return m_OnSlash;
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

	void SetShootDir(const Vector3& Dir)
	{
		m_ShootDir = Dir;
	}

	const Vector3& GetShootDir()	const
	{
		return m_ShootDir;
	}

	class CAnimationMeshComponent* GetAnimationMeshComponent() const;

	void OnHitBack();
	void OnHitBackEnd();
	void OnHitRecoverEnd();
	void OnRoll();
	void OnRollEnd();
	void OnEnableAttackCollider();
	void OnDisableAttackCollider();
};


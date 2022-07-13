#pragma once

#include "Component/ObjectComponent.h"

class CObjectDataComponent :
    public CObjectComponent
{
	friend class CGameObject;

protected:
	CObjectDataComponent();
	CObjectDataComponent(const CObjectDataComponent& com);
	virtual ~CObjectDataComponent();

private:
	ObjectData m_Data;

public:
	void SetHP(int HP)
	{
		m_Data.HP = HP;
	}

	void SetMP(int MP)
	{
		m_Data.MP = MP;
	}

	void SetMoveSpeed(float Speed)
	{
		m_Data.MoveSpeed = Speed;
	}

	void SetAttack(int Attack)
	{
		m_Data.Attack = Attack;
	}

	int GetHP()	const
	{
		return m_Data.HP;
	}

	int GetMP()	const
	{
		return m_Data.MP;
	}

	float GetMoveSpeed()	const
	{
		return m_Data.MoveSpeed;
	}

	int GetAttack()	const
	{
		return m_Data.Attack;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CObjectDataComponent* Clone();


public:
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;
};


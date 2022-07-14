#pragma once

#include "Component/ObjectComponent.h"
#include "Component/NavAgent.h"
#include "GameObject/GameObject.h"

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

	void SetDetectRange(int DetectRange)
	{
		m_Data.DetectRange = DetectRange;
	}

	void SetMoveSpeed(float Speed)
	{
		m_Data.MoveSpeed = Speed;

		CNavAgent* Agent = m_Object->FindObjectComponentFromType<CNavAgent>();

		if (Agent)
		{
			Agent->SetMoveSpeed(m_Data.MoveSpeed);
		}
	}

	void SetAttack(int Attack)
	{
		m_Data.Attack = Attack;
	}

	int GetHP()	const
	{
		return m_Data.HP;
	}

	int GetDetectRange()	const
	{
		return m_Data.DetectRange;
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


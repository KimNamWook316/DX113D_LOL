#pragma once
#include "Component/ColliderSphere.h"

class CMonsterPathFindCollider :
    public CColliderSphere
{
	friend class CGameObject;

protected:
	CMonsterPathFindCollider();
	CMonsterPathFindCollider(const CMonsterPathFindCollider& com);
	virtual ~CMonsterPathFindCollider();

private:
	bool m_PathFindEnable;
	bool m_PathFindCoolStart;
	float m_AccTime;
	float m_PathFindCoolTime;

public:
	void SetPathFindCoolStart(const CollisionResult& Result)
	{
		m_PathFindCoolStart = true;
	}

	void SetPathFindCoolEnd()
	{
		m_PathFindCoolStart = false;
	}

	void SetPathFindFalse(const CollisionResult& Result)
	{
		m_PathFindEnable = false;
		m_PathFindCoolStart = false;
		m_AccTime = 0.f;
	}

	bool GetPathFindEnable()	const
	{
		return m_PathFindEnable;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CMonsterPathFindCollider* Clone();


public:
	virtual bool Save(FILE* File);
	virtual bool Load(FILE* File);
	virtual bool SaveOnly(FILE* File) override;
	virtual bool LoadOnly(FILE* File) override;
};


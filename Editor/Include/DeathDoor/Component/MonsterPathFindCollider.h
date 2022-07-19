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
	class CNavAgent* m_NavAgent;

public:
	void SetPathFindCoolStart(const CollisionResult& Result)
	{
		m_PathFindCoolStart = true;
	}

	void SetPathFindCoolEnd()
	{
		m_PathFindCoolStart = false;
	}

	void SetPathFindFalse(const CollisionResult& Result);

	bool GetPathFindEnable()	const
	{
		return m_PathFindEnable;
	}

	// 다른 몬스터와 충돌한다면 겹치지 않게 밀어내기
	void PushEachOther(const CollisionResult& Result);

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


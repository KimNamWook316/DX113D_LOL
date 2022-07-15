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


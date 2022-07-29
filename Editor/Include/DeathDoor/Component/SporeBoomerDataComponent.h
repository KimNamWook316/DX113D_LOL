#pragma once

#include "MonsterDataComponent.h"

class CSporeBoomerDataComponent :
    public CMonsterDataComponent
{
    friend class CGameObject;

private:
    CSporeBoomerDataComponent();
    CSporeBoomerDataComponent(const CSporeBoomerDataComponent& com);
    virtual ~CSporeBoomerDataComponent();

public:
	virtual void Start() override;

public:
    void OnShootBullet();
};


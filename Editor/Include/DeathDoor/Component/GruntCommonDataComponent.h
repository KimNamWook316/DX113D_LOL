#pragma once
#include "MonsterDataComponent.h"

class CGruntCommonDataComponent :
    public CMonsterDataComponent
{
    friend class CGameObject;

private:
    CGruntCommonDataComponent();
    CGruntCommonDataComponent(const CGruntCommonDataComponent& com);
    virtual ~CGruntCommonDataComponent();

public:
    virtual void Start() override;

};


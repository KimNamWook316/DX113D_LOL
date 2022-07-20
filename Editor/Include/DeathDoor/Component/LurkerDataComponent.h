#pragma once

#include "MonsterDataComponent.h"

class CLurkerDataComponent :
    public CMonsterDataComponent
{
    friend class CGameObject;

private:
	CLurkerDataComponent();
	CLurkerDataComponent(const CLurkerDataComponent& com);
	virtual ~CLurkerDataComponent();

public:
	virtual void Start() override;
};


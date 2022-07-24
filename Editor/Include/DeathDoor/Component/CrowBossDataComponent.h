#pragma once

#include "MonsterDataComponent.h"

class CCrowBossDataComponent :
    public CMonsterDataComponent
{
	friend class CGameObject;

private:
	CCrowBossDataComponent();
	CCrowBossDataComponent(const CCrowBossDataComponent& com);
	virtual ~CCrowBossDataComponent();

public:
	virtual void Start() override;
};


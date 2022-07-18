#pragma once

#include "Component\Node\ConditionNode.h"

class CPostAttackDelayCheck :
    public CConditionNode
{
protected:
    CPostAttackDelayCheck();
    CPostAttackDelayCheck(const CPostAttackDelayCheck& Node);
    virtual ~CPostAttackDelayCheck();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
};


#pragma once

#include "Component\Node\ConditionNode.h"

class CBossKnightFinalAttackCheck :
    public CConditionNode
{
protected:
    CBossKnightFinalAttackCheck();
    CBossKnightFinalAttackCheck(const CBossKnightFinalAttackCheck& Node);
    virtual ~CBossKnightFinalAttackCheck();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;
};


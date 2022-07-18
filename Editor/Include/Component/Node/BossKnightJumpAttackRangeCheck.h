#pragma once

#include "Component\Node\ConditionNode.h"

class CBossKnightJumpAttackRangeCheck :
    public CConditionNode
{
protected:
    CBossKnightJumpAttackRangeCheck();
    CBossKnightJumpAttackRangeCheck(const CBossKnightJumpAttackRangeCheck& Node);
    virtual ~CBossKnightJumpAttackRangeCheck();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
};


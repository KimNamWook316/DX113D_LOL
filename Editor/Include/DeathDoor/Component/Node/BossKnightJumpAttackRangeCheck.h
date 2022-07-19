#pragma once

#include "Component\Node\ConditionNode.h"

class CBossKnightJumpAttackRangeCheck :
    public CConditionNode
{
    friend class CGameBehaviorTree;

protected:
    CBossKnightJumpAttackRangeCheck();
    CBossKnightJumpAttackRangeCheck(const CBossKnightJumpAttackRangeCheck& Node);
    virtual ~CBossKnightJumpAttackRangeCheck();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;
};


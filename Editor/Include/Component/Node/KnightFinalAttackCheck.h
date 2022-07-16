#pragma once

#include "Component\Node\ConditionNode.h"

class CKnightFinalAttackCheck :
    public CConditionNode
{
protected:
    CKnightFinalAttackCheck();
    CKnightFinalAttackCheck(const CKnightFinalAttackCheck& Node);
    virtual ~CKnightFinalAttackCheck();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;
};


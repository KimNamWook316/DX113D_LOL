#pragma once
#include "Component\Node\ConditionNode.h"

class CBossBettyCloseAttackSequenceCheck :
    public CConditionNode
{
protected:
    CBossBettyCloseAttackSequenceCheck();
    CBossBettyCloseAttackSequenceCheck(const CBossBettyCloseAttackSequenceCheck& Node);
    virtual ~CBossBettyCloseAttackSequenceCheck();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;
};


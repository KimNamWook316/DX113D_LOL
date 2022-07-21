#pragma once
#include "Component\Node\ConditionNode.h"

class CRollInputCheck :
    public CConditionNode
{
public:
    CRollInputCheck();
    CRollInputCheck(const CRollInputCheck& Node);
    virtual ~CRollInputCheck();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);
};


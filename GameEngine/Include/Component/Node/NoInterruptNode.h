#pragma once
#include "ConditionNode.h"

class CNoInterruptNode :
    public CConditionNode
{
public:
    CNoInterruptNode();
    CNoInterruptNode(const CNoInterruptNode& Node);
    virtual ~CNoInterruptNode();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);


public:
    virtual bool Invoke(float DeltaTime);
};


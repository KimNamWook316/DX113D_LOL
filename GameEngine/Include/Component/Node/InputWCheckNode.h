#pragma once
#include "ConditionNode.h"

class CInputWCheckNode :
    public CConditionNode
{
public:
    CInputWCheckNode();
    CInputWCheckNode(const CInputWCheckNode& Node);
    virtual ~CInputWCheckNode();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime);
};


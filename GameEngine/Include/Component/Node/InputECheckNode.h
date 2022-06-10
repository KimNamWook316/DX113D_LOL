#pragma once
#include "ConditionNode.h"

class CInputECheckNode :
    public CConditionNode
{
public:
    CInputECheckNode();
    CInputECheckNode(const CInputECheckNode& Node);
    virtual ~CInputECheckNode();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual NodeResult Invoke(float DeltaTime);
};


#pragma once

#include "ConditionNode.h"

class CInputRCheckNode :
    public CConditionNode
{
public:
    CInputRCheckNode();
    CInputRCheckNode(const CInputRCheckNode& Node);
    virtual ~CInputRCheckNode();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual NodeResult Invoke(float DeltaTime);
};


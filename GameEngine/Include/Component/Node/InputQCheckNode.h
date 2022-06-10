#pragma once
#include "ConditionNode.h"

class CInputQCheckNode :
    public CConditionNode
{
public:
    CInputQCheckNode();
    CInputQCheckNode(const CInputQCheckNode& Node);
    virtual ~CInputQCheckNode();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual NodeResult Invoke(float DeltaTime);
};


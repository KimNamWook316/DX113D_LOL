#pragma once

#include "ConditionNode.h"

class CMoveInputCheckNode :
    public CConditionNode
{
public:
    CMoveInputCheckNode();
    CMoveInputCheckNode(const CMoveInputCheckNode& Node);
    virtual ~CMoveInputCheckNode();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

private:
    int m_FrameCount;

public:
    virtual NodeResult Invoke(float DeltaTime);
};


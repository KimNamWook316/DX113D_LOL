#pragma once

#include "ActionNode.h"

class CMoveNode :
    public CActionNode
{
    friend class CBehaviorTree;

public:
    CMoveNode();
    CMoveNode(const CMoveNode& Node);
    virtual ~CMoveNode();

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime) override;
};


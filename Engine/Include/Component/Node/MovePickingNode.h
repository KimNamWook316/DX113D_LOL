#pragma once
#include "ActionNode.h"

class CMovePickingNode :
    public CActionNode
{
    friend class CBehaviorTree;

public:
    CMovePickingNode();
    CMovePickingNode(const CMovePickingNode& Node);
    virtual ~CMovePickingNode();

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual NodeResult Invoke(float DeltaTime) override;
};


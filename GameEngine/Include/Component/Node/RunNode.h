#pragma once

#include "ActionNode.h"

class CRunNode :
    public CActionNode
{
    friend class CBehaviorTree;

public:
    CRunNode();
    CRunNode(const CRunNode& Node);
    virtual ~CRunNode();

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime) override;
};


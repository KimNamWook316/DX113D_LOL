#pragma once

#include "ActionNode.h"

class CIdleNode :
    public CActionNode
{
    friend class CBehaviorTree;
    
public:
    CIdleNode();
    CIdleNode(const CIdleNode& Node);
    virtual ~CIdleNode();

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime) override;
};


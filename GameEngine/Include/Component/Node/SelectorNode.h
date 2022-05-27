#pragma once

#include "CompositeNode.h"

class CSelectorNode :
    public CCompositeNode
{
    friend class CBehaviorTree;

public:
    CSelectorNode();
    CSelectorNode(const CSelectorNode& Node);
    virtual ~CSelectorNode();

public:
    virtual bool Invoke(float DeltaTime)   override;

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);
};


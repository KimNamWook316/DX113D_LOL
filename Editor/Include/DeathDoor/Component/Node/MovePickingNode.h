#pragma once
#include "Component/Node/ActionNode.h"

class CMovePickingNode :
    public CActionNode
{
    friend class CGameBehaviorTree;

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


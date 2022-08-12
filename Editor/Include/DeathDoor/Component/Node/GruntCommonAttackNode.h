#pragma once
#include "Component\Node\ActionNode.h"

class CGruntCommonAttackNode :
    public CActionNode
{
public:
    CGruntCommonAttackNode();
    CGruntCommonAttackNode(const CGruntCommonAttackNode& Node);
    virtual ~CGruntCommonAttackNode();

public:
    virtual void Init() override;

public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;

private :
    void IncMoveSpeed();
    void ChangeToAttackMainAnim();
};


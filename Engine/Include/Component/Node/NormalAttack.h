#pragma once
#include "ActionNode.h"
class CNormalAttack :
    public CActionNode
{
    friend class CBehaviorTree;

public:
    CNormalAttack();
    CNormalAttack(const CNormalAttack& Node);
    virtual ~CNormalAttack();

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime) override;
};


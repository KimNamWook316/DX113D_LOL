#pragma once

#include "Component\Node\ActionNode.h"

class CBossKnightMeleeAttackNode :
    public CActionNode
{
public:
    CBossKnightMeleeAttackNode();
    CBossKnightMeleeAttackNode(const CBossKnightMeleeAttackNode& Node);
    virtual ~CBossKnightMeleeAttackNode();

public:
    virtual void Init() override;

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual NodeResult Invoke(float DeltaTime) override;

private:
    float m_RotatePerSec;
};
 
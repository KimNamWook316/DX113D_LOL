#pragma once

#include "Component\Node\ActionNode.h"

class CBossKnightJumpAttackNode :
    public CActionNode
{
public:
    CBossKnightJumpAttackNode();
    CBossKnightJumpAttackNode(const CBossKnightJumpAttackNode& Node);
    virtual ~CBossKnightJumpAttackNode();

public:
    virtual void Init() override;

protected:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;

public:
    void OnAttackJumpMove(float DeltaTime);

private:
    float m_RotatePerSec;
    float m_MoveSpeed;
    Vector3 m_FinalMoveDir;
};


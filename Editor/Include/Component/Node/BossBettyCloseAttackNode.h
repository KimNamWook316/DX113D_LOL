#pragma once
#include "Component\Node\ActionNode.h"

// 근거리 공격
// 4가지 방향
// Slash Left, Slash Right, Left, Right
// Slash 의 경우, 해당 방향으로 45도 회전 (서서히)
class CBossBettyCloseAttackNode :
    public CActionNode
{
    public:
        CBossBettyCloseAttackNode();
        CBossBettyCloseAttackNode(const CBossBettyCloseAttackNode& Node);
        virtual ~CBossBettyCloseAttackNode();

    public:
        virtual void Init() override;

    protected:
        virtual NodeResult OnStart(float DeltaTime);
        virtual NodeResult OnUpdate(float DeltaTime);
        virtual NodeResult OnEnd(float DeltaTime);

    private:
        float m_RotatePerSec;
};


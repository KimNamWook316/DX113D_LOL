#pragma once
#include "Component\Node\ActionNode.h"

// 2가지 경우
// 1. HP가 30%, 60%가 되는 순간 --> FirstSlamAnger + Roar / 주변 Particle 눈덩이 Damage
// 2. 근거리 공격 연속 2번 성공시 -> First Slam + Roar
class CBossBettySpecialAttackNode :
    public CActionNode
{
public:
    CBossBettySpecialAttackNode();
    CBossBettySpecialAttackNode(const CBossBettySpecialAttackNode& Node);
    virtual ~CBossBettySpecialAttackNode();

public:
    virtual void Init() override;

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

private:
    float m_RotatePerSec;
};


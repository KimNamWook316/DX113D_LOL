#pragma once
#include "Component\Node\ActionNode.h"

// 1. HP가 30%, 60%가 되는 순간 --> FirstSlamAnger + Roar / 주변 Particle 눈덩이 Damage
class CBossBettyAngrylAttackNode :
    public CActionNode
{
public:
    CBossBettyAngrylAttackNode();
    CBossBettyAngrylAttackNode(const CBossBettyAngrylAttackNode& Node);
    virtual ~CBossBettyAngrylAttackNode();
public:
    virtual void Init() override;

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);
private :
    // 1) BossBettyAngryType::HPDamageAngry
    // 공중에서 Snow Ball 들이 떨어지게 한다.
    void OnBossBettyStartFallingSnowBallEffect();
};


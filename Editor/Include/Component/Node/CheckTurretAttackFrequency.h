#pragma once

#include "Component/Node/ConditionNode.h"

class CCheckTurretAttackFrequency :
    public CConditionNode
{
    friend class CGameBehaviorTree;

public:
    CCheckTurretAttackFrequency();
    CCheckTurretAttackFrequency(const CCheckTurretAttackFrequency& Node);
    virtual ~CCheckTurretAttackFrequency();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

private:
    float m_AccTime;

public:
    virtual NodeResult Invoke(float DeltaTime);
};


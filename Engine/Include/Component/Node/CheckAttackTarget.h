#pragma once
#include "ConditionNode.h"

// GameObject의 AttackTarget이 nullptr이 아니고 평타를 때릴만큼 가까운지
class CCheckAttackTarget :
    public CConditionNode
{
public:
    CCheckAttackTarget();
    CCheckAttackTarget(const CCheckAttackTarget& Node);
    virtual ~CCheckAttackTarget();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime);
};


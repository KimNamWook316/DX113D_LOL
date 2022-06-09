#pragma once
#include "ConditionNode.h"

// GameObject의 AttackTarget이 nullptr이 아니고, AttackTarget과의 거리가 평타를 때릴만큼 가까운지
class CCheckAttackTargetSet :
    public CConditionNode
{
public:
    CCheckAttackTargetSet();
    CCheckAttackTargetSet(const CCheckAttackTargetSet& Node);
    virtual ~CCheckAttackTargetSet();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime);
};


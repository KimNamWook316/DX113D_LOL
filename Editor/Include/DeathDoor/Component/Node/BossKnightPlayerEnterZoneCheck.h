#pragma once

#include "Component/Node/ConditionNode.h"

class CBossKnightPlayerEnterZoneCheck :
    public CConditionNode
{
    friend class CGameBehaviorTree;

protected:
    CBossKnightPlayerEnterZoneCheck();
    CBossKnightPlayerEnterZoneCheck(const CBossKnightPlayerEnterZoneCheck& Node);
    virtual ~CBossKnightPlayerEnterZoneCheck();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;
};


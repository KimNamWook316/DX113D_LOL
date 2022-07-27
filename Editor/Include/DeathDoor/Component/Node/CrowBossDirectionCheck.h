#pragma once

#include "Component/Node/ConditionNode.h"

class CCrowBossDirectionCheck :
    public CConditionNode
{
public:
    CCrowBossDirectionCheck();
    CCrowBossDirectionCheck(const CCrowBossDirectionCheck& Node);
    virtual ~CCrowBossDirectionCheck();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;
};


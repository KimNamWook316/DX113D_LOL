#pragma once

#include "Component\Node\ActionNode.h"

class CCrowBossShootNode :
    public CActionNode
{
public:
    CCrowBossShootNode();
    CCrowBossShootNode(const CCrowBossShootNode& Node);
    virtual ~CCrowBossShootNode();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;
};


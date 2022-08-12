#pragma once
#include "Component\Node\ActionNode.h"

class CGruntCommonSpawnCheckNode :
    public CActionNode
{
public:
    CGruntCommonSpawnCheckNode();
    CGruntCommonSpawnCheckNode(const CGruntCommonSpawnCheckNode& Node);
    virtual ~CGruntCommonSpawnCheckNode();

public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;
};


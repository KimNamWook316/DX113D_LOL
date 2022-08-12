#pragma once
#include "Component\Node\ActionNode.h"

class CGruntCommonWalkNode :
    public CActionNode
{
public:
    CGruntCommonWalkNode();
    CGruntCommonWalkNode(const CGruntCommonWalkNode& Node);
    virtual ~CGruntCommonWalkNode();

public:
    virtual void Init() override;
private :
    float m_RotateChangeTime;
    float m_RotateChangeTimeMax;
public:
    virtual NodeResult OnStart(float DeltaTime) override;
    virtual NodeResult OnUpdate(float DeltaTime) override;
    virtual NodeResult OnEnd(float DeltaTime) override;
};


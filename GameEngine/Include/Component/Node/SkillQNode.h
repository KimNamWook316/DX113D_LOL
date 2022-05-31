#pragma once
#include "ActionNode.h"

class CSkillQNode :
    public CActionNode
{
    friend class CBehaviorTree;

public:
    CSkillQNode();
    CSkillQNode(const CSkillQNode& Node);
    virtual ~CSkillQNode();

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime) override;
};


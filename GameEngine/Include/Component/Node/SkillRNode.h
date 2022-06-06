#pragma once

#include "ActionNode.h"

class CSkillRNode :
    public CActionNode
{
    friend class CBehaviorTree;

public:
    CSkillRNode();
    CSkillRNode(const CSkillRNode& Node);
    virtual ~CSkillRNode();

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime) override;
};


#pragma once
#include "ActionNode.h"

class CSkillWNode :
    public CActionNode
{
    friend class CBehaviorTree;

public:
    CSkillWNode();
    CSkillWNode(const CSkillWNode& Node);
    virtual ~CSkillWNode();

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime) override;
};


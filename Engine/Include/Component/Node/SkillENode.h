#pragma once
#include "ActionNode.h"

class CSkillENode :
    public CActionNode
{
    friend class CBehaviorTree;

public:
    CSkillENode();
    CSkillENode(const CSkillENode& Node);
    virtual ~CSkillENode();

protected:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual NodeResult Invoke(float DeltaTime) override;
};


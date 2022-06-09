#pragma once
#include "ConditionNode.h"

class CSkillEndCheckNode :
    public CConditionNode
{
public:
    CSkillEndCheckNode();
    CSkillEndCheckNode(const CSkillEndCheckNode& Node);
    virtual ~CSkillEndCheckNode();

public:
    virtual NodeResult OnStart(float DeltaTime);
    virtual NodeResult OnUpdate(float DeltaTime);
    virtual NodeResult OnEnd(float DeltaTime);

public:
    virtual bool Invoke(float DeltaTime);
};

